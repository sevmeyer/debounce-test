#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


//---- TESTED FUNCTION ----
bool debounce(bool signal);
//-------------------------


#define ITERATIONS  1000000
#define MAX_BOUNCE  8
#define NOISE_PARAMS  {{0,0}, {20,10}, {10,2}, {5,1}}


uint64_t getRandom()
{
    // https://en.wikipedia.org/wiki/Xorshift#xorshift*
    static uint64_t x = 42u;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    return x * UINT64_C(0x2545F4914F6CDD1D);
}


uint32_t getRandomRange(uint32_t min, uint32_t max)
{
    // https://www.pcg-random.org/posts/bounded-rands.html
    const uint64_t random = getRandom() >> 32;
    const uint64_t range = max - min + 1;
    return min + ((random * range) >> 32);
}


uint32_t nextBounce(uint32_t maxBits)
{
    return getRandomRange(0, (UINT32_C(1) << maxBits) - 1);
}


uint32_t nextNoise(uint32_t avgPeriod, uint32_t minGap)
{
    if (avgPeriod == 0)
        return 0;

    const uint32_t minPeriod = minGap + 1;
    const uint32_t maxPeriod = avgPeriod*2 - minGap - 1;

    static uint32_t index = 0;
    uint32_t noise = 0;

    while (index < 32u) {
        noise |= UINT32_C(1) << index;
        index += getRandomRange(minPeriod, maxPeriod);
    }

    index -= 32u;
    return noise;
}


uint32_t getDebounced(uint32_t signal)
{
    uint32_t result = 0;

    for (uint32_t i = 0; i < 32u; ++i) {
        const bool bit = (signal >> i) & 1u;
        result |= debounce(bit) << i;
    }

    return result;
}


uint32_t getDebounceDelay(uint32_t signal, bool state)
{
    uint32_t delay = 32u;

    for (uint32_t i = 0; i < 32u; ++i) {
        const bool bit = (signal >> i) & 1u;

        if (delay >= 32u) {
            if (bit == state)
                delay = i;
        }
        else {
            if (bit != state)
                return 32u;
        }
    }

    return delay;
}


void printBits(uint32_t bits)
{
    for (uint32_t i = 0; i < 32u; ++i) {
        const bool bit = (bits >> i) & 1u;
        printf(bit ? "1" : "-");
    }
}


int main()
{
    printf("------------------------------------------------------\n");
    printf("Debounce test (iterations=%u bounce=%u)\n", ITERATIONS, MAX_BOUNCE);
    printf("------------------------------------------------------\n");

    const uint32_t noiseParams[][2] = NOISE_PARAMS;
    const uint32_t noiseCount = sizeof(noiseParams) / sizeof(noiseParams[0]);

    for (uint32_t n = 0; n < noiseCount; ++n) {
        const uint32_t noisePeriod = noiseParams[n][0];
        const uint32_t noiseMinGap = noiseParams[n][1];

        if (noisePeriod)
            printf("Noise avg=1/%-2u minGap=%-2u  ", noisePeriod, noiseMinGap);
        else
            printf("Noise avg=None minGap=0   ");

        uint32_t maxDelay = 0;
        uint64_t delaySum = 0;
        uint32_t prevSignal = 0;
        uint32_t prevResult = 0;

        for (uint32_t i = 0; i < ITERATIONS; ++i) {
            const uint32_t target = UINT32_C(-1) * (i & 1u);
            const uint32_t bounce = nextBounce(MAX_BOUNCE);
            const uint32_t noise  = nextNoise(noisePeriod, noiseMinGap);
            const uint32_t signal = target ^ bounce ^ noise;
            const uint32_t result = getDebounced(signal);
            const uint32_t delay  = getDebounceDelay(result, target);

            if (delay < 32u) {
                delaySum += delay;
                if (delay > maxDelay)
                    maxDelay = delay;
            }
            else {
                printf("[FAIL]\n");
                printf("\n  Signal: "); printBits(prevSignal); printf(" "); printBits(signal);
                printf("\n  Result: "); printBits(prevResult); printf(" "); printBits(result);
                printf("\n\n");
                return EXIT_FAILURE;
            }

            prevSignal = signal;
            prevResult = result;
        }

        const double avgDelay = 1.0*delaySum / ITERATIONS;
        printf("[ OK ]  Delay avg=%-4.1f max=%u\n", avgDelay, maxDelay);
    }

    return EXIT_SUCCESS;
}
