#include "main.h"

// FlipWait debounce algorithm
// Based on the integrator algorithm by Kenneth A. Kuhn
// https://www.kennethkuhn.com/electronics/debounce.c
//
// In addition to a hysterisis counter, a timeout counter is
// used to temporarily ignore the signal after a state change.
// This allows for an eager transition with low latency, while
// still providing robust noise filtering. All data is stored
// in a single byte per switch.
//
// .-------.-----.-. Inverted
// |7 6 5 4|3 2 1|0| switch state
// '-------'-----'-'
// Timeout  Hysterisis
// counter  counter


#define FLIP 3
#define WAIT 9

#define FLIP_ONE (1u << 1)
#define WAIT_ONE (1u << 4)


bool debounce(bool signal)
{
    static uint8_t state = 0;

    if (state >= WAIT_ONE)
        state -= WAIT_ONE;
    else {
        if (signal != state % 2) {
            if (state >= FLIP_ONE)
                state -= FLIP_ONE;
        }
        else {
            state += FLIP_ONE;
            if (state >= FLIP_ONE*FLIP)
                state =  WAIT_ONE*WAIT + !signal;
        }
    }

    return !(state % 2);
}


// Optional sanity checks

#include <assert.h>
static_assert(FLIP*FLIP_ONE <= WAIT_ONE, "");
static_assert(WAIT*WAIT_ONE < UINT8_MAX, "");
