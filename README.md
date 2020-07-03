Debounce algorithm test
=======================

This repository provides a test harness for debounce algorithms,
which are used to compensate for the [contact bounce] of mechanical
switches. The goal is to verify the correctness and compare the
performance of the given algorithm implementations.

For this purpose, a test class produces a repeatable pseudo-random
test signal. The signal includes flips with random bounces, as well
as noise spikes to simulate electromagnetic interference:

	111111111111111111111111111000000000000000000000001000000000
	001111111111111111111111111101000000000010000000000000000100
	111111111111111011111111111111111111111111111101000000000100
	000000000000000000000000110011111111111111111111101000000000
	000100000000000000000000000011001111111111111111011111111111
	111111111111111101000000000000010000000000000000000011111...

[contact bounce]: https://en.wikipedia.org/wiki/Debounce#Contact_bounce


Rules
-----

Signal rules:

- The initial signal state is 1
- A bounce lasts for at most 5 ticks
- Flips are separated by at least 25 ticks
- Noise lasts for at most 1 tick
- Noise is separated by at least 25 ticks

Reporting rules:

- Each flip must be reported exactly once
- Each flip must be reported before the next flip happens
- Noise spikes must be ignored

A tick can be considered equivalent to one millisecond.


Interface
---------

Algorithms perform the debouncing in a loop function:

	void loop(Test& test)

The `Test` object provides three relevant functions:

	int read()              // Get signal state (0 or 1)
	void sleep(int ticks)   // Progress time
	void report(int state)  // Report a state flip


Build
-----

On Linux, the tests can be compiled and run with:

	mkdir build
	cd build
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make
	./debounce [FLIPCOUNT]
