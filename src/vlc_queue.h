#ifndef VLC_QUEUE_H
#define VLC_QUEUE_H

typedef struct {
    char title[128];
    int season;
    int episode;
} NewEpisode;

typedef struct {
    NewEpisode data[64];
    int head;
    int tail;
} NewEpisodeQueue;

extern NewEpisodeQueue vlc_new_queue;

void queue_init(void);  // <-- add this

int queue_push(NewEpisodeQueue *q, NewEpisode ep);
int queue_pop(NewEpisodeQueue *q, NewEpisode *ep);

#endif /* VLC_QUEUE_H */
