# Week 1 - Reverse

Crackmes - Reverse Engineering Fundamentals

## Build
```bash
make          # compile all crackmes
make clean    # clean
```

## Levels

| # | Binary | Level | Technique | Suggested tools | Status |
|---|--------|-------|-----------|------------------|--------|
| 01 | crackme01_strings | Beginner | Static strings | `strings`, `rabin2 -zz` | Solved |
| 02 | crackme02_xor | Easy | XOR decoding | radare2, pwntools | Solved |
| 03 | crackme03_antidebug | Medium | Anti-debug bypass | gdb, LD_PRELOAD | Solved |
| 04 | crackme04_packed | Medium+ | RAM unpacking | gdb, memory dump | Challenge appears broken, see SOLUTIONS.md |
| 05 | crackme05_crypto | Hard | Hash preimage | Z3 solver, bruteforce | Solved |
| Bonus | wallpaper | Hard | Accumulator rotate/XOR transform, no stored password | meet-in-the-middle search | Solved |

Answers, writeups, and the reasoning behind each solve are in [SOLUTIONS.md](SOLUTIONS.md). Solver scripts for 02/04/05 are in `tools/`; the LD_PRELOAD bypass for 03 is `tools/bypass_ptrace.c`. The bonus challenge is `bonus/wallpaper`, disassembly in `bonus/wallpaper_annotated.txt`.

## Flags to collect

Each crackme prints a flag in the format `CYBERSUP{...}`, to be documented in the report.

## Optional UPX extension

To practice on real packing:
```bash
gcc -O0 -o crackme04_upx src/crackme01_strings.c
upx --best crackme04_upx
file crackme04_upx      # shows "packed by UPX"
upx -d crackme04_upx    # standard unpack
```
