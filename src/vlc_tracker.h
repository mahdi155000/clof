#ifndef VLC_TRACKER_H
#define VLC_TRACKER_H

void* vlc_tracker_thread(void* arg);
void vlc_track_and_update(const char *vlc_password);

#endif /* VLC_TRACKER_H */
