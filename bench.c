#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "aes.h"

int main(void) {
    uint8_t key[] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
                     0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    uint8_t buf[] = {0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,
                     0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};

    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);

    const long n = 1000000;

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    /* Datenkette: in-place, jede Runde verschlüsselt den Output der vorherigen */
    for (long i = 0; i < n; i++) {
        AES_ECB_encrypt(&ctx, buf);
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    double ms = (t1.tv_sec - t0.tv_sec) * 1000.0
              + (t1.tv_nsec - t0.tv_nsec) / 1000000.0;

    /* Prüfsumme erzwingt Beobachtung von buf -> Schleife kann nicht wegoptimiert werden */
    printf("%ld Verschluesselungen in %.3f ms (Pruefsumme:", n, ms);
    for (int j = 0; j < 16; j++) printf("%02x", buf[j]);
    printf(")\n");

    double bytes = (double)n * 16.0;
    double mbPerSec = (bytes / 1000000.0) / (ms / 1000.0);
    printf("Durchsatz: %.2f MB/s\n", mbPerSec);

    return 0;
}