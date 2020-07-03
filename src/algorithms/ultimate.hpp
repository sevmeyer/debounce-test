#include "../Test.hpp"

namespace ultimate {


// Based on The Ultimate Debouncer by Elliot Williams
// https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/


constexpr uint8_t DELAY   = 2;
constexpr uint8_t PRESS   = 0b11000000;
constexpr uint8_t RELEASE = 0b00000111;
constexpr uint8_t MASK    = PRESS | RELEASE;

uint8_t history = 0xFF;

void loop(Test& test)
{
	history = (history << 1) | test.read();

	if ((history & MASK) == RELEASE) {
		history = 0xFF;
		test.report(1);
	}
	else if ((history & MASK) == PRESS) {
		history = 0x00;
		test.report(0);
	}

	test.sleep(DELAY);
}


} // namespace
