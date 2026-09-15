/* LD_PRELOAD hook to bypass crackme03's anti-debug checks.
 *
 * crackme03 uses two checks:
 * 1. PTRACE_TRACEME: fails if a debugger already holds a ptrace on us.
 * 2. /proc/self/status TracerPid: non-zero if something is tracing us.
 *
 * Note: if you just run the binary directly (no debugger attached), both
 * checks already pass on their own - TRACEME succeeds and detaches, and
 * TracerPid is 0. This hook is only needed if you want to attach gdb/strace
 * *while* the checks are active, e.g. to step through the real binary
 * instead of reading source.
 *
 * Build (Linux):
 * gcc -shared -fPIC -o bypass_ptrace.so bypass_ptrace.c -ldl
 * Use:
 * LD_PRELOAD=./bypass_ptrace.so ./crackme03_antidebug
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

long ptrace(enum __ptrace_request request, ...) {
 /* Always report success so is_debugged_ptrace() thinks it's untraced */
 (void)request;
 return 0;
}

static ssize_t (*real_read)(int, void *, size_t) = NULL;

ssize_t read(int fd, void *buf, size_t count) {
 if (!real_read) real_read = dlsym(RTLD_NEXT, "read");
 ssize_t n = real_read(fd, buf, count);
 if (n > 0) {
 char *p = memmem(buf, n, "TracerPid:", 10);
 if (p) {
 /* zero out the digits following "TracerPid:\t" so atoi() reads 0 */
 char *digit = p + 10;
 while (digit < (char *)buf + n && (*digit == '\t' || *digit == ' ')) digit++;
 while (digit < (char *)buf + n && *digit >= '0' && *digit <= '9') *digit++ = '0';
 }
 }
 return n;
}
