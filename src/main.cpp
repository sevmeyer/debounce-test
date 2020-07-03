#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Test.hpp"

#include "algorithms/bitshift.hpp"
#include "algorithms/counter.hpp"
#include "algorithms/delay.hpp"
#include "algorithms/integrator.hpp"
#include "algorithms/pingpong.hpp"
#include "algorithms/stateshift.hpp"
#include "algorithms/ultimate.hpp"


struct Algorithm
{
	std::string name;
	std::function<void(Test&)> loop;
};


std::vector<Algorithm> algorithms{
	{"delay",      delay::loop},
	{"counter",    counter::loop},
	{"integrator", integrator::loop},
	{"pingpong",   pingpong::loop},
	{"bitshift",   bitshift::loop},
	{"ultimate",   ultimate::loop},
	{"stateshift", stateshift::loop}
};


int main(int argc, char* argv[])
{
	std::size_t flips{1000};
	if (argc == 2)
		flips = std::stoul(argv[1]);

	std::ostringstream pass{};
	std::ostringstream fail{};

	for (auto algorithm : algorithms) {
		Test test{60};

		try {
			while (test.getFlips() <= flips)
				algorithm.loop(test);

			auto mean{test.getMeanLatency()};
			auto sdev{test.getSampleStdDevLatency()};
			auto rel{sdev / mean * 100.0};

			pass
				<< std::fixed << std::setprecision(1)
				<< std::left  << std::setw(12) << algorithm.name
				<< std::right << "Latency"
				<< " min:"    << std::setw(3) << test.getMinLatency()
				<< "  max:"   << std::setw(3) << test.getMaxLatency()
				<< "  mean:"  << std::setw(5) << mean
				<< "  sdev:"  << std::setw(5) << sdev
				<< " (" << std::setprecision(0) << rel << "%)\n";
		}
		catch (const std::runtime_error& error) {
			fail
				<< std::left << std::setw(12) << algorithm.name
				<< "FAIL: " << error.what() << "\n"
				<< "  Clean: " << test.getCleanLog() << "\n"
				<< "  Dirty: " << test.getDirtyLog() << "\n\n";
		}
	}

	std::cout
		<< "Debounce algorithm test (" << flips << " flips)\n\n"
		<< pass.str() << "\n"
		<< fail.str();
}
