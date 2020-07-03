#include "../Test.hpp"

namespace integrator {


// Based on debounce.c by Kenneth A. Kuhn
// https://www.kennethkuhn.com/electronics/debounce.c

// The algorithm is extended to detect state
// changes and trigger appropriate reports.


constexpr uint8_t DELAY = 1;
constexpr uint8_t FLIP  = 6;

uint8_t state = 1;
uint8_t count = FLIP;

void loop(Test& test)
{
	// Update count
	if (test.read() == 0) {
		if (count > 0)
			--count;
	}
	else if (count < FLIP)
		++count;

	// Update state
	if (count == 0) {
		if (state != 0)
			test.report(0);
		state = 0;
	}
	else if (count >= FLIP) {
		if (state != 1)
			test.report(1);
		state = 1;
	}

	test.sleep(DELAY);
}


} // namespace
