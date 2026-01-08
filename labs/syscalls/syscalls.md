# Syscalls lab notes

## unknown_syscall

Probes syscall numbers that don't exist in the current macOS kernel, logging the errno responses (typically `ENOSYS`). [web:51][web:54]

## syscall_boundary

Tests edge cases: negative syscall numbers, zero, and passing NULL pointers to observe kernel validation behavior. [web:51][web:74]

## gen_syscalls_table.py

Parses XNU's `bsd/kern/syscalls.master` to generate a C table of syscall numbers and names for cross-reference. [web:53][web:59]

Download XNU source from: https://github.com/apple-oss-distributions/xnu
