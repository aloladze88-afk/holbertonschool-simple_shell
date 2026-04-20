# Simple Shell 0.1

## Description
A simple UNIX command line interpreter.

## Features
- Displays a prompt
- Executes commands using execve
- Handles errors
- Handles EOF (Ctrl+D)

## Limitations
- No PATH handling
- No arguments
- No built-ins
- No pipes or redirections

## Compilation
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o shell

## Usage
./shell
