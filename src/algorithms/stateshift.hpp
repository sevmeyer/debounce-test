#include "../Test.hpp"

namespace stateshift {


// A variation of the Alternative Routine by Jack Ganssle
// http://www.ganssle.com/debouncing-pt2.htm

// To save memory and provide resilience against noise, the
// active state is stored in the upper bit(s), while the
// most recent signal history is stored in the lower bits.


constexpr uint8_t DELAY = 1;
constexpr uint8_t UPPER = 0b11000000;
constexpr uint8_t LOWER = UPPER ^ 0xFF;

uint8_t bits = 0xFF;

void loop(Test& test)
{
	const uint8_t state = bits & UPPER;
	const uint8_t history = (bits << 1) & LOWER;

	bits = state | history | test.read();

	if      (bits == UPPER) { bits = 0x00; test.report(0); }
	else if (bits == LOWER) { bits = 0xFF; test.report(1); }

	test.sleep(DELAY);
}


} // namespace
