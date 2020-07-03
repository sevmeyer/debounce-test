#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <deque>
#include <random>
#include <stdexcept>
#include <vector>


class Test
{
	public:

		Test(std::size_t logSize) :
			logSize_{logSize}
		{}

		// Algorithm interface
		// -------------------

		int read() const
		{
			return dirtyLog_.back();
		}

		void sleep(int ticks)
		{
			for (int s{0}; s < ticks; ++s)
				tick();
		}

		void report(int state)
		{
			if (state != 0 && state != 1)
				throw std::runtime_error{"Invalid report " + toString(state)};
			if (flips_ == 0)
				throw std::runtime_error{"Reported " + toString(state) + " before first flip"};
			if (state == reportState_)
				throw std::runtime_error{"Duplicate " + toString(state) + " report"};
			if (state != cleanLog_.back())
				throw std::runtime_error{"Incorrect " + toString(state) + " report"};

			reportState_ = state;
			latencies_.push_back(ticks_ - prevFlip_);
		}

		// Analysis interface
		// ------------------

		std::size_t getFlips() const
		{
			return flips_;
		}

		std::size_t getMinLatency() const
		{
			return *std::min_element(latencies_.begin(), latencies_.end());
		}

		std::size_t getMaxLatency() const
		{
			return *std::max_element(latencies_.begin(), latencies_.end());
		}

		double getMeanLatency() const
		{
			double sum{0.0};
			for (auto lat : latencies_)
				sum += lat;
			return sum / latencies_.size();
		}

		double getSampleStdDevLatency() const
		{
			double sum{0.0};
			double mean{getMeanLatency()};
			for (auto lat : latencies_)
				sum += (lat - mean) * (lat - mean);
			return std::sqrt(sum / (latencies_.size() - 1));
		}

		std::string getCleanLog() const
		{
			return toString(cleanLog_);
		}

		std::string getDirtyLog() const
		{
			return toString(dirtyLog_);
		}

	private:

		using Dist = std::uniform_int_distribution<std::size_t>;

		std::mt19937 prng_{1};
		Dist randFlip_{25, 50};
		Dist randBounce_{1, 5};
		Dist randState_{0, 1};
		Dist randNoise_{25, 50};

		std::size_t ticks_{0};
		std::size_t flips_{0};
		std::size_t prevFlip_{0};
		std::size_t nextFlip_{randFlip_.min()};
		std::size_t nextNoise_{randNoise_.max()};
		std::size_t bounceEnd_{0};

		int reportState_{1};
		std::vector<std::size_t> latencies_{};

		std::size_t logSize_;
		std::deque<int> cleanLog_{1};
		std::deque<int> dirtyLog_{1};

		void tick()
		{
			++ticks_;
			int state{cleanLog_.back()};

			// Flip
			if (ticks_ >= nextFlip_) {
				if (reportState_ != state)
					throw std::runtime_error{"Missed " + toString(state) + " report"};
				state = !state;
				++flips_;
				prevFlip_ = nextFlip_;
				nextFlip_ += randFlip_(prng_);
				bounceEnd_ = ticks_ + randBounce_(prng_);
			}

			push(cleanLog_, state);

			// Bounce
			if (ticks_ < bounceEnd_)
				state = randState_(prng_);

			// Noise
			if (ticks_ >= nextNoise_) {
				state = !state;
				nextNoise_ += randNoise_(prng_);
			}

			push(dirtyLog_, state);
		}

		void push(std::deque<int>& log, int state)
		{
			log.push_back(state);
			if (log.size() > logSize_)
				log.pop_front();
		}

		std::string toString(int state) const
		{
			return state == 0 ? "0" : "1";
		}

		std::string toString(const std::deque<int>& log) const
		{
			std::string str{};
			for (int state : log)
				str += toString(state);
			return str;
		}
};
