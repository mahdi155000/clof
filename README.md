# Clof – Terminal Movie & Series Manager

**Clof** is a lightweight, terminal-based movie and series manager built with C and ncursesw. It allows you to track movies, TV series, and episodes, with full database persistence using SQLite3. It features a powerful **plugin system** for extensibility, a **TUI interface**, and flexible commands.

---

## Features

* **Interactive TUI (Text User Interface)**
  Navigate your movie and series collection in the terminal with keyboard controls.

* **Series Tracking**
  Track TV series by season and episode. Increment (`next_episode`) or decrement (`prev_episode`) episodes easily.

* **Plugin System**
  Extend functionality with plugins for adding, removing, or updating movies and series. Plugins auto-register and can be executed from the command line or popup menus.

* **SQLite Database Persistence**
  All movies and series are stored in a SQLite database (`movies.db`). Data is automatically saved and loaded on program start and exit.

* **Popup Windows for Commands**
  Commands like `add`, `update`, or `remove` open focused popup windows in the TUI to guide user input.

* **Flexible Command System**
  Press `:` in the TUI to enter commands (like `add`, `remove`, `update`) with interactive popup windows.

* **Mark Movies Watched**
  Easily toggle watched status for movies or series episodes.

* **UTF-8 Support**
  Display checkmarks and special characters properly in the terminal.

---

## Installation

1. **Dependencies**:

   * `gcc` (C Compiler)
   * `ncursesw` library
   * `sqlite3` library
   * `libcurl` library
   * `cJSON` library
   * `pthread`

   On Debian/Ubuntu:

   ```bash
   sudo apt install build-essential libncursesw5-dev libsqlite3-dev libcurl4-openssl-dev libcjson-dev
   ```

2. **Clone the Repository**:

   ```bash
   git clone <your-repo-url>
   cd clof
   ```

3. **Build**:

   ```bash
   make
   ```

4. **Run**:

   ```bash
   ./clof
   ```

---

## Usage

### Keyboard Shortcuts (TUI)

* `UP/DOWN` – Navigate movies and series.
* `ENTER` – Open menu for selected item.
* `n` – Number mode to increment/decrement episodes.
* `:` – Command mode to execute plugins (`add`, `remove`, `update`, etc.).
* `q` – Quit Clof.

### Commands (Plugins)

* `add` – Add a new movie or series with popup input window.
* `update` – Update the episode number of a series.
* `remove` – Remove a movie or series.
* `reid` – Custom plugin for re-identification (if implemented).

---

## File Structure

```
clof/
├── main.c                # Main entry point
├── Makefile              # Build automation
├── movie.h / movie.c     # Movie and series structs and helpers
├── db.h / db.c           # SQLite3 database interface
├── src/
│   ├── vlc_tracker.c     # VLC series tracking logic
│   ├── vlc_queue.c       # Queue management for VLC updates
│   └── ...
├── plugins/
│   ├── add.c
│   ├── remove.c
│   ├── update.c
│   └── ...               # All plugins
└── movies.db             # SQLite database (auto-generated)
```

---

## Plugin Development

Plugins are simple C functions registered automatically:

```c
#include "../plugin.h"
#include "../movie.h"

void plugin_example(WINDOW *parent_win) {
    // Implement your logic with a focused popup window
}

// Register plugin automatically
__attribute__((constructor))
static void register_me(void) {
    register_plugin("example", plugin_example);
}
```

* Plugins take a `WINDOW *parent_win` argument to render inside a popup.
* Auto-registration ensures they appear in the `:` command menu.

---

## Contributing

* Fork the repository and create your branch for new features.
* Keep `master` clean; you can use feature branches for experimental changes.
* Ensure code is well-documented and follows the existing style.
* Test plugins with the TUI interface for proper window rendering.

---

## License

MIT License – free for personal and commercial use.

---

## Screenshots

![Clof Main TUI](screenshots/clof_main.png)
![Clof Popup Add Movie](screenshots/clof_add.png)

---

## Notes

* Ensure your terminal supports UTF-8 for proper display of checkmarks and characters.
* Closing the program saves all changes automatically to `movies.db`.
* Use popup windows for commands to prevent input conflicts in the main screen.

**Clof** – a fast, flexible, terminal movie manager for developers and power users.

