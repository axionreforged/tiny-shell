# tiny-shell

A small Unix-like shell in C built while studying **OSTEP**, focused on learning how shells use **processes**, **fork/exec/wait**, **built-ins**, **file descriptors**, and **basic redirection/pipeline logic**.

## Motivation

This project exists to learn operating-system ideas by building them:

- process creation with `fork()`
- program execution with `execvp()`
- parent/child synchronization with `wait()` / `waitpid()`
- why `cd` must be a built-in
- how file descriptors make redirection possible
- how pipes connect one process’s output to another process’s input

## Current Status

### Implemented
- prompt loop
- input reading with `fgets`
- input normalization
  - remove trailing newline
  - trim leading/trailing spaces
- tokenization into `argv`-style arrays
- built-in `exit`
- built-in `cd`
- single-command external execution using `fork`, `execvp`, and `waitpid`
- basic output redirection with `>`
- basic input redirection with `<`
- basic parsing support for a single `|` pipe
- token memory cleanup after each loop
- single-pipe execution (`cmd1 | cmd2`)

## Supported Commands

### Built-ins
- `exit`
- `cd`

### External commands
Examples:
- `pwd`
- `ls`
- `echo hello`
- `uname`

## Examples

### Simple command
prompt> echo hello
hello