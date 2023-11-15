Debounce test
=============

This repository provides a test harness for debounce functions.
The harness is implemented in C, and produces a digital signal
with pseudo-random contact bounces and noise interferences.

I have set up this harness to experiment with debounce functions
for keyboard firmware. Check the `src` directory for examples.
In particular, I use the EvenOdd algorithm for the [Chrumm] keyboard.
The lowest latency is achieved by the FlipWait algorithm, at the
expense of more complexity.

[Chrumm]: https://github.com/sevmeyer/chrumm-keyboard/


Signal
------

The test signal imitates a simple SPST-NO push switch
(Single Pole, Single Throw, Normally Open), as commonly used
for mechanical keyboards. It is assumed to be connected to a
digital input pin with a pull-up resistor. Therefore, the pin
reads `1` when the switch is open, and `0` when pressed. It is
further assumed that the pin is sampled once per millisecond.
Each switch event must be detected within 32 ticks.

          Switch   .--[ R ]-- V+
           __|__   |
    GND ---O   O---+--------- Pin


Bounce
------

Relevant bounce statistics have been published in
[A Guide to Debouncing] by Jack Ganssle, with an
average bounce time of 1.6 ms and a maximum of 6.2 ms.
[Switch Bouncing Around] by David Ashton reports
reasonably similar results, with a maximum of 3 ms.

Keyboard switch manufacturers [Gateron] and [Kailh] specify
a maximum bounce time of 5 ms. [Cherry] used to do the same,
but nowadays markets "typical" bounce times of less than 1 ms.

The test signal has a maximum bounce time of 8 ticks,
to add a bit of a safety margin.

[A Guide to Debouncing]: https://web.archive.org/web/20230924002022/http://www.ganssle.com/debouncing.htm
[Switch Bouncing Around]: https://www.eeweb.com/switch-bouncing-around/
[Gateron]: https://www.gateron.co/pages/gateron-g-pro-switch-datasheet
[Kailh]: https://www.kailh.net/pages/product-datasheet
[Cherry]: https://web.archive.org/web/20141205193432/http://www.cherrycorp.com/english/switches/key/mx.htm


Noise
-----

In addition to bounces, one must also anticipate random noise,
as explained in [A Guide to Debouncing] by Jack Ganssle, or
the [Ultimate Guide to Switch Debounce] by Max Maxfield.
Therefore, the tests are run multiple times with different
noise saturations. This is rather arbitrary of course, but
should serve as a useful sanity check.

Noise avgPeriod=20 (5%) minGap=10:

    -----1----------------------1---
    -----------------------1--------
    -----------1--------------------
    ----1-------------------1-------

Noise avgPeriod=10 (10%) minGap=2:

    -------1--------------1---------
    1--------------1---------1------
    -------1--1-----------1-----1---
    ---------1---------------1------

Noise avgPeriod=5 (20%) minGap=1:

    -1-----1-1-1-----1----1----1----
    --1---1-----1------1---1---1--1-
    ----1---1----1--1--1-----1---1-1
    ------1---1-------1-------1-----

[Ultimate Guide to Switch Debounce]: https://www.eejournal.com/article/ultimate-guide-to-switch-debounce-part-2/


Build
-----

Initialize the build directory and compile everything with:

    cmake -B build
    cmake --build build

Each function is compiled to a separate binary.
To compile a specific function only:

    cmake --build build --target test-evenodd
