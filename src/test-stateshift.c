#include "main.h"

// StateShift debounce algorithm
// Based on the Alternative Routine by Jack Ganssle
// https://www.ganssle.com/debouncing-pt2.htm
//
// To detect both press and release events, and to
// guard against noise, the active switch state is
// stored in the upper bit, while the most recent
// signal history is stored in the lower bits.


#define UPPER 0x80  // 0b10000000
#define LOWER 0x7F  // 0b01111111


bool debounce(bool signal)
{
    static uint8_t state = 0xFF;

    const uint8_t current = state & UPPER;
    const uint8_t history = (state << 1) & LOWER;

    state = current | history | signal;

    // Events are triggered when the history is saturated
    // with the opposite of the active switch state.
    // As a result, the events are guaranteed to alternate.

    switch (state) {
        case UPPER: state = 0x00; break;
        case LOWER: state = 0xFF; break;
    }

    return state & UPPER;
}
