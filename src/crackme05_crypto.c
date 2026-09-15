/* Crackme 05 - Hard level
 * Goal: reverse a custom hash algorithm and find a collision/preimage
 * Solution: identify the algorithm (rolling hash), constrained bruteforce
 * Tools: Z3 SMT solver OR directed bruteforce
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint32_t custom_hash(const char *s) {
    uint32_t h = 0x1337BEEF;
    while (*s) {
        h = ((h << 5) | (h >> 27)) ^ (uint32_t)(*s);
        h = h * 0x01000193u;
        s++;
    }
    return h;
}

int main(void) {
    char input[128];
    printf("[Crackme 05] Find preimage for hash 0xDEADBABE: ");
    if (!fgets(input, sizeof(input), stdin)) return 1;
    input[strcspn(input, "\n")] = 0;
    uint32_t h = custom_hash(input);
    if (h == 0xDEADBABE) {
        printf("[+] Flag: CYBERSUP{z3_solv3d_th3_h4sh_%08x}\n", h);
        return 0;
    }
    printf("[-] Hash: 0x%08X (need 0xDEADBABE)\n", h);
    return 1;
}
