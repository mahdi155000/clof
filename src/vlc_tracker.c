#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

// Callback to collect HTTP response into a string buffer
static size_t write_callback(void *data, size_t size, size_t nmemb, void *user) {
    size_t total = size * nmemb;
    strncat((char *)user, (char *)data, total);
    return total;
}

// Fetch VLC status.json into buffer
int fetch_vlc_status(const char *password, char *buffer, size_t buffer_len) {
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

// Parse VLC JSON and extract fields
void parse_vlc_status(const char *json) {
    cJSON *root = cJSON_Parse(json);
    if (!root) {
        fprintf(stderr, "Error parsing JSON\n");
        return;
    }

    cJSON *state = cJSON_GetObjectItem(root, "state");
    cJSON *time  = cJSON_GetObjectItem(root, "time");
    cJSON *length = cJSON_GetObjectItem(root, "length");

    printf("VLC state   : %s\n", state ? state->valuestring : "N/A");
    printf("Playback time: %d\n", time ? time->valueint : -1);
    printf("Total length: %d\n", length ? length->valueint : -1);

    // Navigate: root -> information -> category -> meta
    cJSON *info = cJSON_GetObjectItem(root, "information");
    if (info) {
        cJSON *cat = cJSON_GetObjectItem(info, "category");
        if (cat) {
            cJSON *meta = cJSON_GetObjectItem(cat, "meta");
            if (meta) {
                cJSON *showName = cJSON_GetObjectItem(meta, "showName");
                cJSON *season   = cJSON_GetObjectItem(meta, "seasonNumber");
                cJSON *episode  = cJSON_GetObjectItem(meta, "episodeNumber");

                printf("Show name    : %s\n",
                    showName ? showName->valuestring : "unknown");
                printf("Season       : %s\n",
                    season ? season->valuestring : "unknown");
                printf("Episode      : %s\n",
                    episode ? episode->valuestring : "unknown");
            }
        }
    }

    cJSON_Delete(root);
}

int main(void) {
    // VLC web interface password (set in VLC preferences)
    const char *vlc_password = "testpass";

    // Big enough to hold the JSON
    char json_buf[64 * 1024] = {0};

    // Fetch status
    if (fetch_vlc_status(vlc_password, json_buf, sizeof(json_buf)) == 0) {
        parse_vlc_status(json_buf);
    } else {
        fprintf(stderr, "Failed to fetch VLC status\n");
    }

    return 0;
}
