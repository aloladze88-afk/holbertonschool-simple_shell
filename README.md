# Simple Shell (hsh)

## Overview
`hsh` is a minimalist UNIX shell written in C. It reads one command line at a time, tokenizes it, resolves the executable path, spawns a child process, runs the command with `execve`, and waits for it to finish.  
This project is designed to illustrate the fundamentals of shell behavior: input handling, basic parsing, process creation, environment usage, and exit statuses.

---

## Why this project
The purpose of this shell is educational. It focuses on how a shell works internally without implementing advanced parsing or syntactic sugar. The code is intentionally small and readable, so each component maps to one core responsibility.

---

## Features
`hsh` supports both interactive mode and non‑interactive mode.  
It accepts commands given as absolute paths (e.g. `/bin/ls`) or resolves them using the `PATH` environment variable.  
Arguments are separated by spaces or tabs only.  
Two built‑ins are implemented:

- `exit` to terminate the shell
- `env` to print the current environment

The shell exits cleanly on `Ctrl+D`, and errors are reported in a shell‑like format.

---

## Limitations
This version does not implement:

- pipes, redirections, semicolons, quotes, or wildcard expansion
- logical operators (`&&`, `||`)
- command history
- `exit` arguments
- environment manipulation (`setenv`, `unsetenv`)

---

## Requirements
- Ubuntu 20.04 LTS
- GCC (GNU89)
- Betty style
- No memory leaks

---

## Compilation
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

---

## Usage

### Interactive mode
```bash
./hsh
```

### Non‑interactive mode
```bash
echo "/bin/echo hello shell" | ./hsh
```

---

## Example session
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

---

## Code structure (high‑level)

### 1) Main loop (`main.c`)
- Displays the prompt if running interactively.
- Reads input with `getline`.
- Trims the newline and ignores blank lines.
- Checks for built‑ins first.
- If not a built‑in, dispatches to the execution layer.

### 2) Built‑ins (`builtins.c`)
- `exit`: stops the shell and returns the last status.
- `env`: prints all environment variables from `environ`.

### 3) Command execution (`execute.c`)
- Tokenizes input with `strtok`.
- Resolves the executable (via `resolve_command`).
- Forks and calls `execve` in the child.
- Waits for completion in the parent.
- Propagates exit codes (126/127 for errors).

### 4) PATH resolution (`path.c`)
- If the command contains `/`, it is treated as a direct path.
- Otherwise, each directory in `PATH` is tested.
- Properly handles “not found” and “permission denied”.

---

## Exit status
The shell always returns the exit status of the last executed command, which makes it usable in scripts.

---

## Files
- `main.c` – interactive loop and input handling  
- `builtins.c` – built‑in commands  
- `execute.c` – fork/exec/wait logic  
- `path.c` – PATH search and validation  
- `main.h` – shared headers and prototypes  
- `man_1_simple_shell` – manual page  
- `README.md` – documentation  
- `flowchart.mmd` – flowchart diagram  

---

## Flowchart
The following diagram reflects the exact runtime flow used in this project:

```mermaid
flowchart TD
    %% Nodes
    A([Start]):::terminator
    B{Interactive?}:::decision
    C[Print prompt $]:::process
    D[/Read line getline/]:::input
    E{EOF?}:::decision
    F([Exit return last status]):::terminator
    G{Blank line?}:::decision
    H[Check built-ins]:::process
    I[Print environment]:::output
    J[Resolve command PATH/absolute]:::process
    K{Found?}:::decision
    L[Print error + set status]:::process
    M[ Fork ]:::process
    N[Child: execve]:::process
    O[Parent: waitpid]:::process

    %% Flow
    A --> B
    B -- Yes --> C
    B -- No --> D
    C --> D
    D --> E
    E -- Yes --> F
    E -- No --> G
    G -- Yes --> C
    G -- No --> H
    H -- exit --> F
    H -- env --> I
    H -- no --> J
    I --> C
    J --> K
    K -- No --> L
    K -- Yes --> M
    L --> C
    M --> N
    M --> O
    N --> C
    O --> C

    %% Styles
    classDef terminator fill:#1f2937,stroke:#111827,color:#ffffff,stroke-width:2px;
    classDef process fill:#e5e7eb,stroke:#374151,color:#111827,stroke-width:1.5px;
    classDef decision fill:#fff7ed,stroke:#f97316,color:#9a3412,stroke-width:2px;
    classDef input fill:#ecfeff,stroke:#0ea5e9,color:#0f172a,stroke-width:1.5px;
    classDef output fill:#f0fdf4,stroke:#22c55e,color:#14532d,stroke-width:1.5px;
```

---

## Authors
Aleksandre Loladze  
Nicolas Jacquemin
