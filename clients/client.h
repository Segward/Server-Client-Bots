// client.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "curl/include/curl/curl.h"

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *data) {
    size_t realsize = size * nmemb;
    char *response = (char *)data;
    strncat(response, ptr, realsize);
    return realsize;
}

void get_request(char *url, char *response, size_t response_size) {
    CURL *curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        exit(1);
    }

    // Set the URL for the request
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the callback function to handle the response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    // Perform the request
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

void post_request(char *url, char *data, char *response, size_t response_size) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        exit(1);
    }

    // Set the URL for the request
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the callback function to handle the response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    // Set the request method to POST
    curl_easy_setopt(curl, CURLOPT_POST, 1);

    // Set the POST data
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    // Set the Content-Type header
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Perform the request
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    // Clean up
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}