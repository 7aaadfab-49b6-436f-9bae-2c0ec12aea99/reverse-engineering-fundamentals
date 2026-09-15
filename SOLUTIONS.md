# Solutions

A caveat that applies to all five: every flag is a `printf` string literal baked into `.rodata`, so `strings ./binary` (or `radare2 -c izz`) surfaces every flag directly regardless of whether you actually solved the intended technique. The solutions below work the actual mechanism each challenge is testing rather than just grepping the flag out - see `tools/` for the scripts used.

## Crackme 01 - Strings (solved)

**Question:** Find the password via static analysis.

**Answer:** Password is hardcoded in the binary as a plaintext `const char*` - no obfuscation. This one genuinely is just `strings`/`izz`, that's the whole technique being taught.

- Password: `CyberSup_M2_Reverse_Rocks!`
- Flag: `CYBERSUP{w3lc0m3_t0_r3v3rs3}`

## Crackme 02 - XOR (solved)

**Question:** Recover the password from `cipher[]` XORed against repeating key `"KEY"`.

**Answer:** `password[i] = cipher[i] ^ key[i % 3]` for each of the 15 bytes. See `tools/solve_crackme02.js`.

- Password (hex, contains a non-printable byte): `5e 76 5a 68 50 6d 5c 60 45 6d 4a 7f 44 72 4d`
- As chars: `^vZhPm\`EmJ` + `DEL` (0x7F) + `rM` - the 12th byte is DEL (0x7F), not typable at a terminal prompt, which is why the challenge suggests pwntools (`p.sendline(bytes([...]))`) rather than typing it by hand.
- Flag: `CYBERSUP{x0r_1s_w34k_g0t_1t}`

## Crackme 03 - Antidebug (solved)

**Question:** Bypass the `ptrace`/`TracerPid` anti-debug checks and find the password.

**Answer:** Password is hardcoded (`antidebug_bypassed`), same static-analysis flaw as crackme01. The actual anti-debug lesson: both checks only fire if something is *already* tracing the process:
- `is_debugged_ptrace()`: `PTRACE_TRACEME` fails only if a debugger already holds ptrace on you.
- `is_debugged_status()`: `TracerPid` in `/proc/self/status` is non-zero only under an active tracer.

So running the binary directly (no debugger attached) already passes both checks with zero effort - the "bypass" is only needed if you specifically want to run it *under* gdb/strace. For that case, `tools/bypass_ptrace.c` is an `LD_PRELOAD` hook that stubs `ptrace()` to always report success and patches `TracerPid` to `0` on any `read()` of `/proc/self/status`. Not compiled/tested here - no Linux toolchain in this environment (Windows, no gcc/gdb) - but it's the standard technique matching the challenge's own hint.

- Password: `antidebug_bypassed`
- Flag: `CYBERSUP{ptr4c3_byp4ss3d_w1th_lr_pr3l0ad}`

## Crackme 04 - Packed (partially solved - challenge appears broken)

**Question:** Decode the XOR-0xAA-packed blob at runtime and supply it as input.

**Finding:** The source comment claims the blob decodes to `"check_flag_pwned"`. It doesn't - decoding `encrypted_func` with key `0xAA` byte-by-byte gives `43 64 63 63 4b 6e 6c 64 6d 4b 70 7b 6e 63 63 62 aa` = `"CdccKnldmKp{nccb"` + byte `0xAA`. The comment is a red herring. See `tools/solve_crackme04.js`.

**Why it can't be fully resolved statically:** `encrypted_func` is 17 bytes (16 "encoded" + 1 trailing `0x00` in the initializer). The decode loop XORs *all 17* bytes including that trailing one, turning it into `0xAA` - a non-zero byte. That means the decoded buffer has no null terminator within its declared bounds, so `strcmp(input, (char*)encrypted_func)` reads past the end of the array into whatever memory happens to follow it. The actual required input therefore depends on runtime memory layout (stack/data segment contents adjacent to a static buffer), which is compiler/build/ASLR dependent and not determinable from source alone - it needs the dynamic analysis the challenge itself points at (`gdb` breakpoint after the unpack loop, dump the real decoded bytes from memory). No Linux/gdb available in this environment to do that dump.

- Decoded (unreliable, does not decode to something typable/meaningful - flags this as likely a bugged challenge rather than a solving gap): `CdccKnldmKp{nccb` + `0xAA`
- Flag (known from the source's printf literal, not independently verified by actually triggering the branch): `CYBERSUP{unp4ck3d_th3_s3cr3t}`

## Crackme 05 - Crypto / hash preimage (solved)

**Question:** Find any input whose `custom_hash()` equals `0xDEADBABE`.

**Answer:** `custom_hash` is a bijection per character - rotate-left-5, XOR with the byte, multiply by odd constant `0x01000193` (invertible mod 2^32) - so it's solvable by meet-in-the-middle rather than raw brute force: build a table of hashes over all 3-character printable-ASCII prefixes forward from the seed `0x1337BEEF`, build a table over all 3-character suffixes inverted backward from the target, and find the shared intermediate state. See `tools/solve_crackme05.js`.

- Input: `],##a"` (6 printable ASCII chars)
- Verified: `custom_hash("],##a\"") == 0xDEADBABE`
- Flag: `CYBERSUP{z3_solv3d_th3_h4sh_deadbabe}`
