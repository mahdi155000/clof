#include <pthread.h>
#include <unistd.h>
#include "vlc_tracker.h"

// Background thread function
void* vlc_tracker_thread(void* arg) {
    const char* password = (const char*)arg;
    
    while (1) {
        vlc_track_and_update(password);
        
        // Wait 1 second before polling again
        sleep(1);
    }
    
    return NULL;
}
