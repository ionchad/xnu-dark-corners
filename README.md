# xnu-dark-corners

Experimental C programs exploring obscure and under-documented behaviors in the macOS XNU kernel (Mach + BSD + IOKit layers). Each lab targets a specific subsystem with multiple tests documenting actual kernel behavior on Intel macOS.

## Labs

### 1. VM
Memory management edge cases: `mmap`, `msync`, `madvise` behavior on anonymous vs file-backed mappings.

**Tests:**
- `msync_weird`: Different flags on anonymous mappings
- `madvise_zero`: `MADV_ZERO` zeroing behavior
- `madvise_dontneed`: Page cache eviction timing

**Key findings:**
- `msync(MS_INVALIDATE)` on anonymous private mappings returns `EINVAL` (errno 22)
- `MADV_ZERO` successfully zeroes all bytes in anonymous mappings

### 2. PTRACE
`ptrace` anti-debug mechanisms and tracing behavior.

**Tests:**
- `ptrace_deny_attach`: `PT_DENY_ATTACH` with isolated attach attempt
- `ptrace_trace_me`: `PT_TRACE_ME` with `SIGSTOP` handling

**Key findings:**
- `PT_ATTACHEXC` on a `PT_DENY_ATTACH` child can kill the tracer process (isolated via double-fork)
- `PT_TRACE_ME` causes child to stop on `SIGSTOP` as expected

### 3. PROC
Process lifecycle, zombies, wait variants, and session/process groups.

**Tests:**
- `zombie_wait4`: Zombie formation and reaping with `wait4`
- `waitid_weird`: `waitid` with different flags and `idtype` values
- `setsid_pgrp`: Session creation and process group signals

**Key findings:**
- Zombies persist until parent calls `wait4`/`waitpid`
- `waitid` with `WNOWAIT` allows multiple waits on same child

### 4. SIGNALS
Signal handling edge cases and race conditions.

**Tests:**
- `sigaltstack`: Alternate signal stack setup and verification
- `sigsuspend_race`: Condition variable-style signal waiting

**Key findings:**
- `sigaltstack` correctly switches to alternate stack during handler execution
- `sigsuspend` atomically unmasks and waits for signals

### 5. SYSCALLS
Raw syscall behavior, invalid syscall numbers, and boundary checks.

**Tests:**
- `unknown_syscall`: Probing non-existent syscall numbers
- `syscall_boundary`: Negative, zero, and high syscall numbers with NULL pointers

**Key findings:**
- Invalid syscall numbers trigger `SIGSYS` rather than just returning `ENOSYS`
- Syscall 500 exists and returns 0 on Intel macOS
- Passing NULL to `open()` returns `EFAULT` (errno 14)

### 6. MMAP-STRANGE
Large file mappings, sparse access patterns, and hint address behavior.

**Tests:**
- `mmap_large_file`: 64 MB file mapping and boundary access
- `mmap_sparse_access`: 128 MB region with 1 MB stride access pattern
- `mmap_hint_addresses`: Testing address hints from low to high memory

**Key findings:**
- `mmap` respects hint addresses at high addresses (0x200000000+)
- Hints below 4 GB are often ignored in favor of higher addresses

### 7. IOKIT
IORegistry enumeration and kernel extension properties.

**Tests:**
- `iokit_enumerate`: Enumerate `IOPlatformExpertDevice` services
- `iokit_properties`: Read model, manufacturer, and serial number from IORegistry

**Key findings:**
- IOKit exposes platform details without requiring root
- IORegistry properties are accessible via `IORegistryEntryCreateCFProperty`

### 8. PTRACE-FIXED
Fully isolated `PT_DENY_ATTACH` test (re-enabled from lab 2 with safety wrapper).

**Tests:**
- `ptrace_deny_attach`: Double-fork isolation to prevent runner crash

**Key findings:**
- Test process dies from signal when attempting to attach to protected child (expected macOS behavior)

### 9. SIDE-CHANNELS
Microarchitectural side-channel primitives on Intel.

**Tests:**
- `cache_timing`: Cached vs uncached memory access timing
- `flush_reload`: Flush+Reload attack simulation
- `prime_probe`: Prime+Probe cache set probing

**Key findings:**
- Cached access: ~10-50 cycles, uncached: ~200-400 cycles (Intel)
- Flush+Reload can detect which cache lines were accessed by victim
- Cache timing side-channels are observable from userspace

### 10. THREAD-RACES
pthread race conditions, synchronization primitives, and thread lifecycle.

**Tests:**
- `mutex_race`: 4 threads incrementing shared counter with mutex
- `cond_spurious_wakeup`: Condition variable spurious wakeup detection
- `pthread_cancel`: Thread cancellation with cleanup handlers

**Key findings:**
- Mutex prevents races: counter reaches exactly 40000
- Condition variables can have spurious wakeups (must check predicate in loop)
- `pthread_cancel` correctly invokes cleanup handlers

## Building

Requires macOS with Xcode Command Line Tools:

```bash
clang -Wall -Wextra -O2 -framework IOKit -framework CoreFoundation -o runner runner.c labs/*/*.c
./runner
