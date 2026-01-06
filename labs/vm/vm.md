# VM lab notes

## msync_weird

- `MS_SYNC` and `MS_ASYNC` on anonymous private mappings succeed on macOS 
- `MS_INVALIDATE` on an anonymous private mapping fails with `EINVAL` (errno 22), suggesting XNU requires a file backed mapping for invalidation. [web:81][web:86]

## madvise_zero

- `MADV_ZERO` on anonymous mappings results in all bytes being zeroed in this experiment (length = 4 pages). [web:84]

## madvise_dontneed

- `MADV_DONTNEED` on a 4 MB shared file mapping slightly increases the time of the next page scan, indicating some eviction or refaulting behavior. [web:76]
