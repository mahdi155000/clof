#ifndef VLC_TRACKER_H
#define VLC_TRACKER_H

#include "vlc_queue.h"

void* vlc_tracker_thread(void* arg);
void vlc_track_and_update(const char *vlc_password);

#endif /* VLC_TRACKER_H */