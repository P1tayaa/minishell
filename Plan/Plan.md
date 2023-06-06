# Notre plan

### list the chose a faire:

#### function allowed:

##### C Function Libraries

###### GNU Readline Library

- `readline`: Reads a line from the terminal and returns it, allowing for interactive editing.
- `rl_clear_history`: Clears the history list in GNU readline.
- `rl_on_new_line`: Informs readline that the cursor is on a new line.
- `rl_replace_line`: Replaces the line currently displayed with a new one.
- `rl_redisplay`: Redraws the current line on the terminal.
- `add_history`: Adds a line to the history list.

###### Standard C Library (libc)

- `printf`: Outputs formatted string to stdout.
- `malloc`: Allocates dynamic memory.
- `free`: Deallocates dynamic memory.
- `exit`: Terminates a program.
- `getcwd`: Gets the current working directory.
- `chdir`: Changes the current working directory.
- `strerror`: Returns a string describing an error number.
- `perror`: Outputs a string and an error message to stderr.
- `getenv`: Gets an environment variable.
- `isatty`: Checks if a file descriptor refers to a terminal.

###### POSIX Library

- `write`: Writes to a file descriptor.
- `access`: Checks the file's access permissions.
- `open`: Opens or creates a file for reading or writing.
- `read`: Reads from a file descriptor.
- `close`: Closes a file descriptor.
- `fork`: Creates a new process.
- `wait`: Waits for a child process to stop or terminate.
- `waitpid`: Waits for a specific child process to stop or terminate.
- `wait3`: Waits for a child process to stop or terminate and returns resource usage.
- `wait4`: Waits for a specific child process to stop or terminate and returns resource usage.
- `signal`: Sets a function to handle a signal.
- `sigaction`: Specifies how a signal should be processed.
- `sigemptyset`: Initializes a signal set to empty.
- `sigaddset`: Adds a signal to a signal set.
- `kill`: Sends a signal to a process.
- `stat`: Gets the status of a file.
- `lstat`: Gets the status of a symbolic link.
- `fstat`: Gets the status of a file through its file descriptor.
- `unlink`: Removes a file.
- `execve`: Executes a program.
- `dup`: Duplicates a file descriptor.
- `dup2`: Duplicates a file descriptor to a specified file descriptor.
- `pipe`: Creates a pipe.
- `opendir`: Opens a directory.
- `readdir`: Reads a directory entry.
- `closedir`: Closes a directory.
- `ioctl`: Controls a device.
- `ttyname`: Returns the file name of the terminal associated with a file descriptor.
- `ttyslot`: Returns the index into the termcap file for the current user.
- `tcsetattr`: Sets the parameters associated with a terminal.
- `tcgetattr`: Gets the parameters associated with a terminal.

###### Termcap Library

- `tgetent`: Loads a terminal capability entry.
- `tgetflag`: Gets the boolean value of a terminal capability.
- `tgetnum`: Gets the numeric value of a terminal capability.
- `tgetstr`: Gets the string value of a terminal capability.
- `tgoto`: Decodes a cursor motion string.
- `tputs`: Applies padding information to a string and outputs it.
