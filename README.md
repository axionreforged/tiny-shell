# tiny-shell

A small Unix-like shell in C built while studying **OSTEP**, focused on learning how shells use **processes**, **fork/exec/wait**, **built-ins**, and **basic file-descriptor redirection**.

## Status

Currently implemented:
- prompt loop
- input reading with `fgets`
- input normalization
  - remove trailing newline
  - trim leading/trailing spaces
- tokenization into `argv`-style arrays
- built-in `exit`
- built-in `cd`
- single-command external execution using `fork`, `execvp`, and `wait`
- basic output redirection with `>`
- basic input redirection with `<`
- token memory cleanup after each loop

Not implemented yet:
- pipes (`|`)
- append redirection (`>>`)
- stderr redirection
- background jobs
- quoting / escaping
- advanced parsing
- job control / signals handling polish

## Motivation

This project is being built as a hands-on companion to **OSTEP** in order to understand:
- how a shell launches programs
- how parent and child processes differ
- how `exec` replaces a process image
- how `wait` synchronizes with children
- how file descriptors make redirection possible
- why some commands such as `cd` must be built-ins

## Features

### Built-ins
- `exit`
- `cd`

### External commands
Examples:
- `pwd`
- `ls`
- `echo hello`
- `uname`

### Redirection
Examples:
- `echo hello > out.txt`
- `cat < out.txt`
- `cat < in.txt > copy.txt`

## Limitations

This shell is intentionally small and incomplete.

Current limitations include:
- only one command at a time
- no piping yet
- simple whitespace-based tokenization
- no support for quoted strings
- fixed token count / token length limits
- simplified built-in behavior
- minimal syntax validation compared to a real shell

## Build

```bash
make