#include "../Test.hpp"

namespace delay {


// A very simple approach that reads the state
// at a lower rate than the expected bounce time.


constexpr uint8_t DELAY = 6;

uint8_t state = 1;

void loop(Test& test)
{
	if (test.read() != state) {
		state = !state;
		test.report(state);
	}

	test.sleep(DELAY);
}


} // namespace
