# holbertonschool-shell_v2

## Description
This project is a simple shell implementation in C, designed to mimic the behavior of the Unix shell. It supports basic commands, input/output redirection, and environment variable management.

## Features
You can then enter commands interactively. The shell supports the following features:
- **Interactive Mode**: The shell prompts for user input and executes commands.
- **Non-Interactive Mode**: The shell can read commands from a file or standard input.
- **Heredoc Mode**: The shell can handle heredoc redirection.
- **Pipeline Mode**: The shell can execute commands in a pipeline.
- **Environment Variables**: The shell can access and modify environment variables.
- **Built-in Commands**: The shell supports built-in commands such as `exit`, `env`, `setenv`, and `unsetenv`.
- **Command Execution**: The shell can execute external commands and handle input/output redirection.

## Compilation
To compile the shell, use the following command:
```bash
make
```
This will create an executable file named `hsh`.

## Documentation
To access the documentation for `hsh`, run the following command:
```bash
man ./man_1_hsh
```

## Usage
To run the shell, execute the following command:
```bash
./hsh
```
## Memory Leaks
To check for memory leaks in `hsh`, run the following command:
```bash
valgrind --leak-check=full --show-leak-kinds=all ./hsh
```

## Bugs
- No known bugs at this time.

## Authors
- Melvin Redondo--Tanis
