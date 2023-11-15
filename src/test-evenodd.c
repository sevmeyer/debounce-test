#include "main.h"

// EvenOdd debounce algorithm
// Based on the integrator algorithm by Kenneth A. Kuhn
// https://www.kennethkuhn.com/electronics/debounce.c
//
// The progress is maintained in one byte per switch.
// The lowest bit represents the inverse of the current
// switch state. It is inverted so that the starting
// value is zero and can be auto-initalized in an array.
// The remaining bits are used as a hysterisis counter.
//
// .-------------.-.
// |7 6 5 4 3 2 1|0|
// '-------------'-'
// Hysterisis    Inverted
// counter       switch state


#define FLIP 6


bool debounce(bool signal)
{
    static uint8_t state = 0;

    // The hysterisis counter reflects how often the signal
    // has matched the state bit in recent history. We add
    // or subtract a value of 2 to jump over the state bit.
    //            .-.
    //          .-' '-. .- Counter
    //    .-. .-'     '-'
    // ---' '-'
    // ! ! = ! = = = ! ! = Signal

    if (signal == state % 2)
        state += 2;
    else if (state >= 2)
        state -= 2;

    // When the signal has fully saturated the counter,
    // invert the state bit and reset the counter.

    if (state >= FLIP * 2)
        state = !signal;

    return !(state % 2);
}
