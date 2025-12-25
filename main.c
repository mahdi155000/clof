#include "stdio.h"
#include "plugin.h"
#include "movie.h"
#include "db.h"
#include <pthread.h>
#include "src/vlc_tracker.h"
#include "src/vlc_queue.h"


extern PluginEntry plugins[];
extern int plugin_count;
// NewEpisodeQueue vlc_new_queue;

// int main(void) {
//     db_init();
//     db_load_movies();

//     plugin_tui();

//     db_save_movies();
//     db_close();
//     return 0;
// }

int main(void) {
    // 1️⃣ Initialize database
    db_init();
    db_load_movies();
    // queue_init(&vlc_new_queue);
    queue_init();
    

    // 2️⃣ Start VLC tracking thread
    const char *vlc_password = "testpass"; // VLC HTTP password
    pthread_t vlc_thread;

    if (pthread_create(&vlc_thread, NULL, vlc_tracker_thread, (void*)vlc_password) != 0) {
        fprintf(stderr, "Error: unable to start VLC tracker thread\n");
    } else {
        // Optional: detach thread so we don't have to join later
        pthread_detach(vlc_thread);
    }

    // 3️⃣ Start the TUI (main user interface)
    plugin_tui();

    // 4️⃣ Save and cleanup
    db_save_movies();
    db_close();

    return 0;
}
