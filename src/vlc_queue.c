#include <pthread.h>
#include <string.h>
#include "vlc_queue.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Global queue accessible outside this file
NewEpisodeQueue vlc_new_queue;

// Initialize the queue
void queue_init(void) {
    vlc_new_queue.head = 0;
    vlc_new_queue.tail = 0;
}

// Push a new episode to the queue
int queue_push(NewEpisodeQueue *q, NewEpisode ep) {
    pthread_mutex_lock(&mutex);
    int next_tail = (q->tail + 1) % 64;
    if (next_tail == q->head) {
        pthread_mutex_unlock(&mutex);
        return 0; // full
    }
    q->data[q->tail] = ep;
    q->tail = next_tail;
    pthread_mutex_unlock(&mutex);
    return 1;
}

// Pop an episode from the queue
int queue_pop(NewEpisodeQueue *q, NewEpisode *ep) {
    pthread_mutex_lock(&mutex);
    if (q->head == q->tail) {
        pthread_mutex_unlock(&mutex);
        return 0; // empty
    }
    *ep = q->data[q->head];
    q->head = (q->head + 1) % 64;
    pthread_mutex_unlock(&mutex);
    return 1;
}
