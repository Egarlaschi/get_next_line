*This project has been created as part of the 42 curriculum by egarlasc.*

# get_next_line

## Description

`get_next_line` is a C function that reads a single line at a time from a file descriptor, returning it each time it is called. It can handle reading from files, standard input, or any valid file descriptor. The function is designed to be called in a loop to retrieve successive lines until the end of the file is reached, at which point it returns `NULL`.

The goal of the project is to understand and implement buffered I/O from scratch, working with static variables, dynamic memory allocation, and the management of a persistent read buffer (the "stash") across multiple function calls.

The project consists of three files:

- `get_next_line.c` — core logic: reading into the stash, extracting a line, and cleaning the stash.
- `get_next_line_utils.c` — helper functions: string utilities reimplemented without using the standard library (`ft_strchr`, `ft_strlen`, `ft_strlcpy`, `ft_strdup`, `ft_strjoin`).
- `get_next_line.h` — header file with prototypes and includes.

## Algorithm

The implementation is built around a **static stash + read-and-accumulate** strategy. Here is a detailed breakdown of the design choices and their justification:

### Static stash

A `static char *stash` is maintained inside `get_next_line()`. Because it is `static`, it persists between calls to the function, allowing leftover data from a previous `read()` call (i.e., bytes that come after a newline) to be remembered and used in the next invocation without re-reading from the file descriptor.

This is necessary because `read()` does not know about newlines — it simply fills the buffer up to `BUFFER_SIZE` bytes. A single `read()` call may contain zero, one, or several newlines, so the excess must be stored somewhere.

### read_and_stash

This function accumulates data from the file descriptor into the stash by repeatedly calling `read()` with a buffer of `BUFFER_SIZE` bytes and appending each chunk to the stash via `ft_strjoin` (which also frees the old stash). The loop continues **until a newline is found in the stash or EOF is reached** (`bytes == 0`).

The loop condition `!ft_strchr(stash, '\n')` checks whether a complete line is already available — if so, no further reading is needed for this call.

**Justification:** This approach minimises unnecessary reads while correctly handling any buffer size, from 1 to very large values. It also handles the edge case where a file contains no trailing newline.

### line_extractor

Once the stash contains at least one complete line (or everything up to EOF), this function allocates a new string containing the content up to and including the `\n` character (or up to the end of the stash if no `\n` is present). The extracted string is returned as the line for the current call.

### stash_cleaner

After the line has been extracted, the stash must be updated to remove the portion that was just returned. This function finds the `\n`, allocates a new string with everything after it, frees the old stash, and returns the trimmed remainder. If no `\n` exists (i.e., the last line of the file was returned), the stash is freed and `NULL` is returned, signalling EOF on the next call.

### Why this design?

The three-step pipeline — **accumulate → extract → trim** — cleanly separates concerns and makes each component independently testable. The use of `ft_strjoin` with internal freeing of `s1` keeps memory management contained within the join step, avoiding double-free issues. The static variable approach is the idiomatic and expected solution for this project, avoiding the need for any global state or external data structures.

## Instructions

### Compilation

`get_next_line` is not a standalone program — it is a library function. To compile and test it, include it in your own `main.c` and compile with the `-D BUFFER_SIZE=<n>` flag to define the read buffer size:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

Replace `42` with any positive integer to change the buffer size.

### Example usage

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("test.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

### Notes

- The function returns `NULL` on error or when there is nothing left to read.
- Each returned line must be `free()`d by the caller.
- The behaviour is undefined if `BUFFER_SIZE` is not defined at compile time.
- Reading from multiple file descriptors alternately is not supported by this single-fd implementation (a bonus version using an array of stashes would be required for that).

## Resources

### Documentation & references

- [man 2 read](https://man7.org/linux/man-pages/man2/read.2.html) — Linux manual page for the `read()` system call.
- [man 3 malloc / free](https://man7.org/linux/man-pages/man3/malloc.3.html) — Memory allocation reference.
- [Static variables in C — GeeksforGeeks](https://www.geeksforgeeks.org/static-variables-in-c/) — Explanation of `static` local variable behaviour across function calls.
- [File descriptors — Wikipedia](https://en.wikipedia.org/wiki/File_descriptor) — Overview of file descriptors in Unix-like systems.
- *The C Programming Language*, Kernighan & Ritchie — foundational reference for C string and I/O handling.

### AI usage

AI was used during this project for the following tasks:

- **README generation:** the structure and content of this README was drafted with the assistance of Claude based on the source code and project requirements.

AI was not used to write the core implementation. All `.c` and `.h` files were authored by `egarlasc`.
