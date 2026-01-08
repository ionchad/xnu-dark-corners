#!/usr/bin/env python3
import sys
import re

def parse_syscalls_master(path):
    """Parse XNU syscalls.master and extract syscall numbers and names."""
    syscalls = []
    with open(path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith(';') or line.startswith('#'):
                continue
            # Format: number   type   name   ...
            match = re.match(r'^(\d+)\s+\w+\s+(\w+)', line)
            if match:
                num, name = match.groups()
                syscalls.append((int(num), name))
    return syscalls

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: gen_syscalls_table.py <path-to-syscalls.master>")
        sys.exit(1)
    
    syscalls = parse_syscalls_master(sys.argv[1])
    print(f"// Generated from syscalls.master - {len(syscalls)} syscalls")
    print("typedef struct { int num; const char *name; } syscall_entry;")
    print("static syscall_entry syscall_table[] = {")
    for num, name in syscalls:
        print(f"    {{ {num}, \"{name}\" }},")
    print("};")
    print(f"static const size_t syscall_table_count = {len(syscalls)};")
