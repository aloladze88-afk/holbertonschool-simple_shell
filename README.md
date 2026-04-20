# Simple Shell (0.2)

## Description
`hsh` is a minimal UNIX command interpreter written in C.

This version:
- reads one command line at a time
- forks a child process
- executes the command with `execve`
- waits for the child to finish
- supports arguments separated by spaces or tabs

## Supported in this version
- interactive mode with the `($) ` prompt
- non-interactive mode from a pipe or file
- absolute-path commands such as `/bin/ls`
- arguments such as `/bin/ls -l /tmp`
- clean exit on `Ctrl+D`
- error reporting when a command cannot be executed

## Not supported in this version
- `PATH` lookup
- built-ins such as `exit` or `env`
- quotes, pipes, redirections, semicolons, or advanced parsing

## Requirements
- Ubuntu 20.04 LTS
- `gcc`
- Betty style compliance
- no memory leaks

## Compilation
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Usage
Interactive mode:
```bash
./hsh
```

Non-interactive mode:
```bash
echo "/bin/echo hello shell" | ./hsh
```

## Example
```text
$ ./hsh
($) /bin/ls -l
total 32
-rw-r--r-- 1 user user  911 execute.c
-rw-r--r-- 1 user user 1106 main.c
($) /bin/echo hello shell
hello shell
($) ls
./hsh: 3: ls: not found
($)
$
```

`ls` fails because this version does not search `PATH`. The shell passes
arguments to `execve`, but it only splits on spaces and tabs.

## Files
- `main.h`: shared prototypes and includes
- `main.c`: shell loop and EOF handling
- `execute.c`: argument splitting, `fork`, `execve`, and `waitpid`
- `README.md`: project description

## Authors
- Aleksandre Loladze
- Nicolas Jacquemin
