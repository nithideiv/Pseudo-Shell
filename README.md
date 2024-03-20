# Project 1: Pseudo-Shell

A shell in computer science is a software program or interface that interprets commands from the user for the operating system to execute. In Unix systems, the shell is a text-based command-line interface allowing users to interact with the system by typing commands. Project 1 requires building a simplified version of such a shell, termed as the "Pseudo-Shell," focusing on file system navigation, manipulation, and I/O operations.

## Project Details

### Shell Modes

The pseudo-shell supports two modes:
- Interactive mode: Users interact with the shell by typing commands into the console.
- File mode: Commands are read from a batch file specified via command line arguments. Output is written to "output.txt."

### Base Shell Functionalities

The shell implements various UNIX-like commands including `ls`, `pwd`, `mkdir`, `cd`, `cp`, `mv`, `rm`, and `cat` using system calls. Error handling includes managing incorrect parameters and syntax errors.

### Project Structure Requirements

The project containa the following files:
- `main.c`: Contains the main function handling both file and interactive modes.
- `command.c` and `command.h`: Implement and declare shell command functions respectively.
- `string_parser.c` and `string_parser.h`: Handle input string parsing.
- `Makefile`: Standard make file to produce the executable `pseudo-shell`.
