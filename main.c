#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>$

#include "md5/md5.h"
#include "collision.h"

#define MD5_SIZE 128/8

#define ACTUAL_SIZE 64/8

void truncated_md5(uint8_t const *input, size_t input_len, uint8_t output[ACTUAL_SIZE]);

bool brents_power_updated(unsigned int power);

typedef char hash_str[3 * ACTUAL_SIZE];
void sprint_bytes_hex(uint8_t const bytes[ACTUAL_SIZE], hash_str str);

int main(int argc, char *argv[])
{
    struct timespec start_time, end_time;
    timespec_get(&start_time, TIME_UTC);

    // initialize rand
    srand((unsigned int) time(NULL));

    // generate random y0
    uint8_t y0[ACTUAL_SIZE];
    for (int i = 0; i < ACTUAL_SIZE; ++i)
    {
        y0[i] = rand();
    }

    hash_str y0_str;
    sprint_bytes_hex(y0, y0_str);
    printf("Random y0: %s\n", y0_str);

    printf("Starting cycle finding...\n");
    uint64_t lambda, mu;
    uint8_t m1[ACTUAL_SIZE];
    uint8_t m2[ACTUAL_SIZE];
    brents_cycle_find(ACTUAL_SIZE, y0, truncated_md5, brents_power_updated, &lambda, &mu, m1, m2);

    printf("lambda: %" PRIu64 "\n", lambda);
    printf("mu:     %" PRIu64 "\n", mu);

    hash_str m1_str, m2_str;
    sprint_bytes_hex(m1, m1_str);
    sprint_bytes_hex(m2, m2_str);
    printf("m1: %s\n", m1_str);
    printf("m2: %s\n", m2_str);

    uint8_t h1[ACTUAL_SIZE], h2[ACTUAL_SIZE];
    truncated_md5(m1, ACTUAL_SIZE, h1);
    truncated_md5(m2, ACTUAL_SIZE, h2);

    if (memcmp(h1, h2, ACTUAL_SIZE) == 0)
    {
        hash_str h_str;
        sprint_bytes_hex(h1, h_str);
        printf("Success!\n");
        printf("Hash: %s\n", h_str);
    }
    else
    {
        printf("Fail!\n");
    }

    timespec_get(&end_time, TIME_UTC);
    printf("Computation took %ld seconds...\n", end_time.tv_sec - start_time.tv_sec);

    return 0;
}

void truncated_md5(uint8_t const *input, size_t input_len, uint8_t output[ACTUAL_SIZE])
{
    md5_byte_t md5_out[MD5_SIZE];
    md5_state_t state;
    md5_init(&state);
    md5_append(&state, input, input_len);
    md5_finish(&state, md5_out);
    memcpy(output, md5_out, ACTUAL_SIZE);
}

void sprint_bytes_hex(uint8_t const bytes[ACTUAL_SIZE], hash_str str)
{
    for (int i = 0; i < ACTUAL_SIZE; ++i)
    {
        str += sprintf(str, "%02X", (unsigned int) bytes[i]);
        if (i != (sizeof(bytes) - 1))
        {
            str += sprintf(str, " ");
        }
    }
}

bool brents_power_updated(unsigned int power)
{
    printf("2^%f\n", log2(power));
    fflush(stdout);
    return true;
}

