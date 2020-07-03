#include "../Test.hpp"

namespace counter {


// Based on the Counting Algorithm by Jack Ganssle
// http://www.ganssle.com/debouncing-pt2.htm

// The count direction is reversed to make it more intuitive.


constexpr uint8_t DELAY = 1;
constexpr uint8_t FLIP  = 6;

uint8_t state = 1;
uint8_t count = 0;

void loop(Test& test)
{
	if (test.read() == state)
		count = 0;
	else if (++count >= FLIP) {
		count = 0;
		state = !state;
		test.report(state);
	}

	test.sleep(DELAY);
}


} // namespace
