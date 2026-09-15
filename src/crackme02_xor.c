/* Crackme 02 - Easy level
 * Goal: password XOR-encoded with a repeating key
 * Solution: analyze the decode loop, extract ciphertext + key
 * Tools: radare2 (afl, pdf), gdb, pwntools
 */
#include <stdio.h>
#include <string.h>

static const unsigned char cipher[] = {
    0x15, 0x33, 0x03, 0x23, 0x15, 0x34, 0x17, 0x25, 0x1C, 0x26, 0x0F, 0x26, 0x0F, 0x37, 0x14
};
static const char *key = "KEY";

int check(const char *input) {
    int len = sizeof(cipher);
    if ((int)strlen(input) != len) return 0;
    for (int i = 0; i < len; i++) {
        if ((input[i] ^ key[i % 3]) != cipher[i]) return 0;
    }
    return 1;
}

int main(void) {
    char input[128];
    printf("[Crackme 02] XOR secret: ");
    if (!fgets(input, sizeof(input), stdin)) return 1;
    input[strcspn(input, "\n")] = 0;
    if (check(input)) {
        printf("[+] Flag: CYBERSUP{x0r_1s_w34k_g0t_1t}\n");
        return 0;
    }
    printf("[-] Nope.\n");
    return 1;
}
