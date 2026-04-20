# Simple Shell (1.0)

## Description
`hsh` is a minimal UNIX command interpreter written in C.

This version:
- reads one command line at a time
- forks a child process
- executes commands with `execve`
- waits for the child to finish
- supports arguments separated by spaces or tabs
- resolves commands using `PATH`
- implements the `exit` and `env` built-ins

## Supported in this version
- interactive mode with the `($) ` prompt
- non-interactive mode from a pipe or file
- absolute-path commands such as `/bin/ls`
- commands found through `PATH` such as `ls`
- arguments such as `/bin/ls -l /tmp`
- the built-in command `exit`
- the built-in command `env`
- clean exit on `Ctrl+D`
- error reporting when a command cannot be executed

## Not supported in this version
- quotes, pipes, redirections, semicolons, or advanced parsing
- handling an argument for `exit`
- environment modification built-ins such as `setenv` or `unsetenv`

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
($) env
USER=julien
HOME=/home/julien
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
($) /bin/echo hello shell
hello shell
($) exit
$
```

## Files
- `main.h`: shared prototypes and includes
- `main.c`: shell loop and EOF handling
- `builtins.c`: built-in command handling for `exit` and `env`
- `execute.c`: argument splitting, `fork`, `execve`, and `waitpid`
- `path.c`: `PATH` lookup and command resolution helpers
- `README.md`: project description
- `man_1_simple_shell`: manual page

## Authors
- Aleksandre Loladze
- Nicolas Jacquemin
