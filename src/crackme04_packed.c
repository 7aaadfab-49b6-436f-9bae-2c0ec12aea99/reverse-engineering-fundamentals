/* Crackme 04 - Medium+ level
 * Goal: "packed" binary (encrypted in RAM, decoded at runtime)
 * Solution: memory dump after unpack, dynamic analysis
 * Tools: gdb with a breakpoint on main, Volatility, runtime ELF dump
 *
 * Note: the "pack" here is symbolic (simple XOR). For real UPX:
 *     upx --best crackme04_packed && upx -d crackme04_packed
 */
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

/* "Hidden" function, encrypted at build time, decoded in RAM */
static unsigned char encrypted_func[] = {
    /* XOR 0xAA of "check_flag_pwned" (misleading, see SOLUTIONS.md) */
    0xE9, 0xCE, 0xC9, 0xC9, 0xE1, 0xC4, 0xC6, 0xCE, 0xC7, 0xE1, 0xDA, 0xD1, 0xC4, 0xC9, 0xC9, 0xC8, 0x00
};

int main(void) {
    for (int i = 0; i < (int)sizeof(encrypted_func); i++) encrypted_func[i] ^= 0xAA;
    char input[128];
    printf("[Crackme 04] Unpack and guess: ");
    if (!fgets(input, sizeof(input), stdin)) return 1;
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, (char *)encrypted_func) == 0) {
        printf("[+] Flag: CYBERSUP{unp4ck3d_th3_s3cr3t}\n");
        return 0;
    }
    printf("[-] Decode the blob!\n");
    return 1;
}
