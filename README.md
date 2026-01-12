# xnu-dark-corners

C programs that works underdocumented behaviors in the XNU kernel on macos

Targets one syscall, flag combination or a kernel path
Logs erros and sucesses or any unsual behavior

## building

```bash
clang -Wall -Wextra -02 -o runner runner.c labs/*.c

running
./runner
