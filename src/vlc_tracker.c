#include <string.h>  
#include <stdlib.h>  
#include "vlc_tracker.h"
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "../movie.h"
#include "vlc_queue.h"  // Include your queue header

static size_t write_callback(void *data, size_t size, size_t nmemb, void *user) {
    strncat((char*)user, (char*)data, size * nmemb);
    return size * nmemb;
}

static int fetch_vlc_status(const char *password, char *buffer, size_t buffer_len) {
    CURL *curl = curl_easy_init();
    if (!curl) return -1;

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/requests/status.json");
    curl_easy_setopt(curl, CURLOPT_USERNAME, "");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK) ? 0 : -1;
}

void vlc_track_and_update(const char *vlc_password) {
    char json_buf[64*1024] = {0};

    if (fetch_vlc_status(vlc_password, json_buf, sizeof(json_buf)) != 0)
        return; // failed to fetch

    cJSON *root = cJSON_Parse(json_buf);
    if (!root) return;

    cJSON *state = cJSON_GetObjectItem(root, "state");
    cJSON *time  = cJSON_GetObjectItem(root, "time");
    cJSON *length = cJSON_GetObjectItem(root, "length");

    cJSON *info = cJSON_GetObjectItem(root, "information");
    if (info) {
        cJSON *cat = cJSON_GetObjectItem(info, "category");
        if (cat) {
            cJSON *meta = cJSON_GetObjectItem(cat, "meta");
            if (meta) {
                cJSON *showName   = cJSON_GetObjectItem(meta, "showName");
                cJSON *seasonStr  = cJSON_GetObjectItem(meta, "seasonNumber");
                cJSON *episodeStr = cJSON_GetObjectItem(meta, "episodeNumber");

                if (showName && seasonStr && episodeStr) {
                    char *title = showName->valuestring;
                    int season = atoi(seasonStr->valuestring);
                    int episode = atoi(episodeStr->valuestring);

                    int idx = movie_exists(title);
                    if (idx != -1) {
                        // Existing movie, update season/episode
                        movies[idx].season  = season;
                        movies[idx].episode = episode;

                        if (time && length &&
                            (length->valueint > 0 &&
                             time->valueint >= (length->valueint - 5))) {
                            movies[idx].watched = 1;
                        }
                    } else {
                        // New movie/episode detected, push to VLC queue
                        NewEpisode ne;
                        strncpy(ne.title, title, sizeof(ne.title)-1);
                        ne.title[sizeof(ne.title)-1] = 0;
                        ne.season  = season;
                        ne.episode = episode;
                        queue_push(&vlc_new_queue, ne);
                    }
                }
            }
        }
    }

    cJSON_Delete(root);
}
