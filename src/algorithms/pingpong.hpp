#include "../Test.hpp"

namespace pingpong {


// A variation of the integrator algorithm by Kenneth A. Kuhn
// https://www.kennethkuhn.com/electronics/debounce.c

// To save memory, the active state is encoded in the count
// variable. Values below the FLIP threshold represent
// the 0 state, values above FLIP represent the 1 state.


constexpr uint8_t DELAY = 1;
constexpr uint8_t FLIP  = 6;
constexpr uint8_t MAX   = 2*FLIP;

uint8_t count = MAX;

void loop(Test& test)
{
	if (test.read() == 0) {
		if (count > 0) {
			if (--count == FLIP) {
				count = 0;
				test.report(0);
			}
		}
	}
	else {
		if (count < MAX) {
			if (++count == FLIP) {
				count = MAX;
				test.report(1);
			}
		}
	}

	test.sleep(DELAY);
}


} // namespace
