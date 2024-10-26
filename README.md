# Minishell

Minishell is a custom shell developed in C as part of the 42 curriculum. It replicates basic shell functionality, allowing for command execution, piping, environment variable handling, and more. This project was a collaborative effort.

## Contributors
- [Omathot](https://github.com/omathot)
- [sboulain](https://github.com/P1tayaa)

## Features
- **Prompt & History**: Displays a prompt and stores a command history.
- **Command Execution**: Finds and executes commands based on `$PATH`, or using relative or absolute paths.
- **Signal Handling**: Handles `ctrl-C`, `ctrl-D`, and `ctrl-\` similar to bash.
- **Quotes & Redirection**:
  - Manages single (`'`) and double (`"`) quotes.
  - Supports input/output redirections (`<`, `>`, `<<`, `>>`) and pipes (`|`).
- **Environment Variables**: Expands `$` variables and `$?` for exit status.
- **Built-ins**: Implements essential commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`).

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/P1tayaa/minishell.git
   cd minishell
   make
   ./minishell
