#include "../Test.hpp"

namespace bitshift {


// Based on the Alternative Routine by Jack Ganssle
// http://www.ganssle.com/debouncing-pt2.htm


constexpr uint8_t DELAY = 1;

uint8_t history = 0xFF;

void loop(Test& test)
{
	history = (history << 1) | test.read() | 0b10000000;

	if      (history == 0b11000000) test.report(0);
	else if (history == 0b10111111) test.report(1);

	test.sleep(DELAY);
}


} // namespace
