# clof - AI Coding Guidelines

## Project Overview
**clof** is a C-based media tracking application (rewrite of "lof") that manages movies and TV series with progress tracking. It uses a plugin-based architecture where each command is implemented as a separate plugin.

## Architecture

### Core Components
- **main.c**: Entry point with command loop that dispatches to plugins
- **movie.c/h**: Core data model and persistence (binary file storage in `movies.db`)
- **plugin.h**: Plugin system interface (function pointer typedef + struct)
- **plugins/*.c**: Individual command implementations

### Plugin System
Each plugin follows this pattern:
```c
void plugin_command(void) {
    // Implementation
}
```
Registered in `plugins/plugins.c` array with name, help text, and function pointer.

### Data Model
```c
typedef struct {
    char title[TITLE_LEN];  // TITLE_LEN = 100
    int is_series;          // 0=movie, 1=series
    int season, episode;    // progress tracking
} Movie;
```

## Build & Run
```bash
make        # builds with gcc -Wall -g, links -lncurses
./clof      # runs CLI: type commands like 'add', 'list', 'tui'
```

## Key Patterns

### Input Handling
- Use `fgets()` + `strcspn(cmd, "\n") = 0` to read lines without newline
- Mix `scanf()` for numbers with `getchar()` to consume newline
- Always check return values from input functions

### Memory Management
- Fixed-size arrays: `Movie movies[MAX_MOVIES]` (MAX_MOVIES=1000)
- String copying: `strncpy(dest, src, TITLE_LEN-1); dest[TITLE_LEN-1] = '\0'`

### Persistence
- Binary file I/O: `fread/fwrite` for struct arrays
- Simple format: count + array of structs

### TUI (plugins/tui.c)
- Uses ncurses for terminal UI
- Modal input: NORMAL, NUMBER, COMMAND modes
- Custom drawing functions for screen updates

### Error Handling
- Minimal: mostly silent failures or basic printf messages
- No exceptions - return early on errors

## Development Workflow
1. Add new command: create `plugins/newcmd.c`, implement `plugin_newcmd()`, register in `plugins/plugins.c`
2. Update Makefile SRC list
3. Test with `make && ./clof`

## File Organization
- Headers in root: `movie.h`, `plugin.h`
- Implementation files in root: `main.c`, `movie.c`
- All plugins in `plugins/` subdirectory
- Build artifacts: `*.o`, `clof` executable in root

## Dependencies
- **ncurses**: Required for TUI plugin (`-lncurses` in Makefile)
- **Standard C libraries**: stdio, string, stdlib

## Common Tasks
- **Add feature**: Create plugin file, register in plugins.c, update Makefile
- **Modify data model**: Update Movie struct, adjust save/load logic
- **Fix input issues**: Check scanf/getchar sequences, buffer sizes