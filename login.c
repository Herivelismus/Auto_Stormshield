#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <pcre.h>
#include <time.h>
#include <libgen.h>
#include <unistd.h>

#define SLEEP_TIME 7 * 60 * 60

size_t writeCallback(char *ptr, size_t size, size_t nmemb, char **output);

char *get_path(int argc, char *argv[]);

int extractSessionToken(const char *htmlContent, char *sessionToken) {
    const char *pattern = "<input type=\"hidden\" name=\"session\" value=\"id=([^&\"]+)";
    const int OVECCOUNT = 30;
    int rc, i;
    int ovector[OVECCOUNT];
    const char *error;
    int erroffset;
    pcre *re;

    re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
    if (re == NULL) {
        return -1;
    }

    rc = pcre_exec(re, NULL, htmlContent, strlen(htmlContent), 0, 0, ovector, OVECCOUNT);
    if (rc < 0) {
        pcre_free(re);
        return -1;
    }

    for (i = ovector[2]; i < ovector[3]; i++) {
        *sessionToken = htmlContent[i];
        sessionToken++;
    }
    *sessionToken = '\0';

    pcre_free(re);
    return 0;
}

size_t writeCallback(char *ptr, size_t size, size_t nmemb, char **output) {
    size_t dataSize = size * nmemb;

    *output = (char *)malloc(dataSize + 1);
    memcpy(*output, ptr, dataSize);
    (*output)[dataSize] = '\0';

    return dataSize;
}

char *get_path(int argc, char *argv[]) {
    char exe_path[1024];
    char dir_path[1024];
    if (realpath(argv[0], exe_path) != NULL) {
        strcpy(dir_path, dirname(exe_path));
        char *path = malloc(strlen(dir_path) + strlen("/credentials.txt") + 1);
        if (path != NULL) {
            strcpy(path, dir_path);
            strcat(path, "/credentials.txt");
            return path;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

int main(int argc, char *argv[]) {
    char *id;
    char *password;
    char *host;

    if (argc == 4) {
        id = strdup(argv[1]);
        password = strdup(argv[2]);
        host = strdup(argv[3]);
    } else {
        char *path = get_path(argc, argv);
        FILE *file = fopen(path, "r");
        if (file == NULL) {
            return 1;
        }
        char buffer[1000];
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            fclose(file);
            return 1;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        id = strdup(buffer);
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            fclose(file);
            return 1;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        password = strdup(buffer);
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            fclose(file);
            return 1;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        host = strdup(buffer);
        fclose(file);
    }

    CURL *curl;
    CURLcode res;

    while (1) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if(curl) {
            char url[1000];
            snprintf(url, sizeof(url), "%s/auth/plain.html", host);
            curl_easy_setopt(curl, CURLOPT_URL, url);

            struct curl_httppost *post = NULL;
            struct curl_httppost *last = NULL;
            curl_formadd(&post, &last,
                         CURLFORM_COPYNAME, "url",
                         CURLFORM_COPYCONTENTS, "",
                         CURLFORM_END);
            curl_formadd(&post, &last,
                         CURLFORM_COPYNAME, "logout",
                         CURLFORM_COPYCONTENTS, "",
                         CURLFORM_END);
            curl_formadd(&post, &last,
                         CURLFORM_COPYNAME, "time",
                         CURLFORM_COPYCONTENTS, "480",
                         CURLFORM_END);
            curl_formadd(&post, &last,
                         CURLFORM_COPYNAME, "uid",
                         CURLFORM_COPYCONTENTS, id,
                         CURLFORM_END);
            curl_formadd(&post, &last,
                         CURLFORM_COPYNAME, "authnum",
                         CURLFORM_COPYCONTENTS, "1",
                         CURLFORM_END);
            curl_formadd(&post, &last,
                         CURLFORM_COPYNAME, "pswd",
                         CURLFORM_COPYCONTENTS, password,
                         CURLFORM_END);

            curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

            char *responseBuffer = NULL;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

            res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                // Handle error if needed
            } else {
                char sessionToken[100];
                if (extractSessionToken(responseBuffer, sessionToken) == 0) {
                    char url[1000];
                    char postFields[1000];
                    snprintf(url, sizeof(url), "%s/auth/disclaimer.html", host);
                    snprintf(postFields, sizeof(postFields), "form1=&form2=&form3=&session=id%%3D%s&url=&time=&authnum=&read=accepted&action=I accept", sessionToken);

                    CURL *curlDisclaimer;
                    curlDisclaimer = curl_easy_init();

                    if(curlDisclaimer) {
                        curl_easy_setopt(curlDisclaimer, CURLOPT_URL, url);
                        curl_easy_setopt(curlDisclaimer, CURLOPT_POSTFIELDS, postFields);
                        curl_easy_setopt(curlDisclaimer, CURLOPT_WRITEFUNCTION, writeCallback);

                        res = curl_easy_perform(curlDisclaimer);
                        
                        if(res != CURLE_OK) {
                            exit(1);
                        }

                        curl_easy_cleanup(curlDisclaimer);
                    }
                } else {
                    exit(1);
                }
                free(responseBuffer);
            }

            curl_easy_cleanup(curl);
            curl_formfree(post);
        }

        curl_global_cleanup();
    printf("%s\n","Logged in !  Sleeping for 7 hours");
        sleep(SLEEP_TIME);
    }

    free(id);
    free(password);
    free(host);

    return 0;
}