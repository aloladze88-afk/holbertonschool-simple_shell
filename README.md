# Simple Shell (0.1)

## Description
`hsh` is a minimal UNIX command interpreter written in C. This is the
0.1 version — the smallest working shell, on which later tasks build.

It reads one command per line, forks a child, runs the command with
`execve`, waits for it to finish, and prompts again.

### Supported in this version
- interactive mode (prompt shown when reading from a terminal)
- non-interactive mode (reading commands from a pipe or a file)
- single-word commands given as absolute paths (e.g. `/bin/ls`, `/bin/date`)
- clean exit on `Ctrl+D` (end of file)
- error reporting when a command cannot be executed

### Not supported in this version
- `PATH` lookup — commands must be given as absolute paths
- arguments to commands
- built-ins such as `exit` or `env`
- pipes, redirections, semicolons, quoting, or any other special characters

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
echo "/bin/ls" | ./hsh
```

## Example
```
$ ./hsh
($) /bin/ls
execute.c  hsh  main.c  main.h  README.md
($) ls
./hsh: No such file or directory
($)
$
```
In the example above, `ls` on its own fails because `PATH` is not used
in this version — the shell only runs commands given as absolute paths.
The final prompt disappears when `Ctrl+D` is pressed.

## Files
- `README.md` — this file
- `main.h` — shared prototypes and includes (include-guarded)
- `main.c` — the prompt/read loop and EOF handling
- `execute.c` — the `fork`/`execve`/`wait` helper

## Authors
Aleksandre Loladze, Nicolas Jacquemin
