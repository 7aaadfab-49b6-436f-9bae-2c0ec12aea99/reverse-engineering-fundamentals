/* Crackme 03 - Medium level
 * Goal: bypass the anti-debug ptrace + TracerPid check
 * Solution: patch ptrace() OR LD_PRELOAD OR gdb set follow-fork-mode
 * Tools: gdb, strace, ltrace, IDA/Ghidra, LD_PRELOAD hooks
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <fcntl.h>

int is_debugged_ptrace(void) {
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) return 1;
    ptrace(PTRACE_DETACH, 0, 0, 0);
    return 0;
}

int is_debugged_status(void) {
    char buf[4096]; int fd = open("/proc/self/status", O_RDONLY);
    if (fd < 0) return 0;
    int n = read(fd, buf, sizeof(buf)-1); close(fd);
    if (n <= 0) return 0; buf[n] = 0;
    char *p = strstr(buf, "TracerPid:");
    if (!p) return 0;
    int pid = atoi(p + 10);
    return pid != 0;
}

int main(void) {
    if (is_debugged_ptrace() || is_debugged_status()) {
        printf("[-] Debugger detected. Bye.\n");
        return 1;
    }
    char input[128];
    printf("[Crackme 03] Secret: ");
    if (!fgets(input, sizeof(input), stdin)) return 1;
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "antidebug_bypassed") == 0) {
        printf("[+] Flag: CYBERSUP{ptr4c3_byp4ss3d_w1th_lr_pr3l0ad}\n");
        return 0;
    }
    printf("[-] Wrong password.\n");
    return 1;
}
