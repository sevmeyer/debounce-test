#include "main.h"

// JustForFun debounce algorithm
// Based on the integrator algorithm by Kenneth A. Kuhn
// https://www.kennethkuhn.com/electronics/debounce.c
//
// What are conditionals?


#define MAX 6


bool debounce(bool signal)
{
    static uint8_t state = 0;

    const uint8_t max = state >= MAX*2;
    const uint8_t two = state >= 2;
    const uint8_t div = state / 2;
    const uint8_t mod = state % 2;

    state = (div + !signal*!max - signal*two)*2 + ((mod&two) | max);

    return !mod;
}
