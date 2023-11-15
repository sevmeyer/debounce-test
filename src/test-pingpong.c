#include "main.h"

// PingPong debounce algorithm
// Based on the integrator algorithm by Kenneth A. Kuhn
// https://www.kennethkuhn.com/electronics/debounce.c
//
// To save memory, the active state is encoded in the count
// variable. Values below the FLIP threshold represent
// the 0 state, values above FLIP represent the 1 state.


#define FLIP 6
#define MAX (FLIP*2)


bool debounce(bool signal)
{
    static uint8_t count = MAX;

    if (signal) {
        if (count < MAX) {
            if (++count == FLIP)
                count = MAX;
        }
    }
    else {
        if (count > 0) {
            if (--count == FLIP)
                count = 0;
        }
    }

    return count > FLIP;
}
