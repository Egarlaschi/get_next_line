*This project has been created as part of the 42 curriculum by egarlasc.*

# get_next_line

## Description

`get_next_line` is a C function that reads one line at a time from a file descriptor, returning it on each call. It works with files, standard input, or any valid file descriptor, and is designed to be called in a loop until the end of the file is reached — at which point it returns `NULL`.

The goal of the project is to understand and implement buffered I/O from scratch, working with static variables, dynamic memory allocation, and a persistent read buffer (the "stash") that survives across multiple function calls.

The project is made up of three files:

- `get_next_line.c` — core logic: reading into the stash, extracting a line, and trimming the stash.
- `get_next_line_utils.c` — helper functions: string utilities reimplemented without the standard library (`ft_strchr`, `ft_strlen`, `ft_strlcpy`, `ft_strdup`, `ft_strjoin`).
- `get_next_line.h` — header with prototypes and includes.

A bonus version (`_bonus` files) supports reading from multiple file descriptors simultaneously using an array of stashes indexed by fd.

---

## Algorithm

The implementation is built around a **static stash + read-and-accumulate** strategy. Here is a breakdown of each component and its design rationale.

### Static stash

A `static char *stash` is kept inside `get_next_line()`. Being static, it persists between calls, so leftover data from a previous `read()` — bytes that come after a newline — is remembered and reused in the next invocation without re-reading from the file descriptor.

This is necessary because `read()` knows nothing about newlines: it fills the buffer up to `BUFFER_SIZE` bytes regardless. A single call may capture zero, one, or several newlines, so the excess must be stored somewhere across calls.

### `read_and_stash`

Accumulates data from the file descriptor into the stash by calling `read()` repeatedly with a buffer of `BUFFER_SIZE` bytes and appending each chunk via `ft_strjoin` (which also frees the old stash). The loop continues until a newline is found in the stash or EOF is reached (`bytes == 0`).

The condition `!ft_strchr(stash, '\n')` short-circuits the loop when a complete line is already available, avoiding unnecessary reads.

**Why this approach:** it handles any buffer size correctly, from 1 byte upward, and naturally deals with files that have no trailing newline.

### `line_extractor`

Once the stash contains at least one complete line (or everything up to EOF), this function allocates a new string with the content up to and including `\n` — or up to the end of the stash if no `\n` is present. That string is returned as the line for the current call.

### `stash_cleaner`

Updates the stash after a line has been returned. It finds the `\n`, allocates a new string with everything after it, frees the old stash, and returns the remainder. If no `\n` exists (the last line of the file was just returned), the stash is freed and `NULL` is returned, signalling EOF on the next call.

### Why this design?

The three-step pipeline — **accumulate → extract → trim** — separates concerns cleanly and keeps each component independently testable. Freeing `s1` inside `ft_strjoin` keeps memory management contained within the join step and avoids double-free issues. The static variable approach is the idiomatic solution for this project, requiring no global state or external data structures.

---

## Instructions

### Compilation

`get_next_line` is a library function, not a standalone program. Include it in your own `main.c` and compile with the `-D BUFFER_SIZE=<n>` flag:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

Replace `42` with any positive integer to set the read buffer size.

### Example

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

- Returns `NULL` on error or when there is nothing left to read.
- Each returned line must be `free()`d by the caller.
- Behaviour is undefined if `BUFFER_SIZE` is not defined at compile time.
- The standard version does not support interleaved reads on multiple file descriptors; use the bonus version for that.

---

## Resources

- [man 2 read](https://man7.org/linux/man-pages/man2/read.2.html) — Linux manual page for the `read()` system call.
- [man 3 malloc / free](https://man7.org/linux/man-pages/man3/malloc.3.html) — Memory allocation reference.
- [Static variables in C — GeeksforGeeks](https://www.geeksforgeeks.org/static-variables-in-c/) — How `static` local variables behave across function calls.
- [File descriptors — Wikipedia](https://en.wikipedia.org/wiki/File_descriptor) — Overview of file descriptors in Unix-like systems.
- *The C Programming Language*, Kernighan & Ritchie — foundational C reference.

### AI usage

AI was used during this project for:

- **README generation:** structure and content drafted with Claude's assistance based on the source code and project requirements.

All `.c` and `.h` files were written by `egarlasc`. AI was not used to write the core implementation.
