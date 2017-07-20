#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "logger.h"

int laji_log_findnewfile();

pthread_mutex_t logger_mutex = PTHREAD_MUTEX_INITIALIZER;
char laji_log_filepath[616]; // right way to save file path?
int laji_log_inited = 0;
int laji_log_filefd = -1;
int laji_log_enabled = 1;

int laji_log_init(const char* path) {
    if (laji_log_enabled) {
        pthread_mutex_init(&logger_mutex, NULL);
        strcpy(laji_log_filepath, path);
        if (laji_log_findnewfile() != -1) {
            laji_log_inited = 1;
            return 0;
        } else {
            fputs("laji_log_init() can not create file for read.\n", stdout);
        }
    }
    return 0;
}

int laji_log_findnewfile() {
    char buffer[616], num_buffer[61];
    int number = 0;

    while(number < 616) {
        sprintf(num_buffer, "_%d", number);
        sprintf(buffer, "%slog%s.log", laji_log_filepath, number == 0 ? "" : num_buffer);
        laji_log_filefd = open(buffer, O_WRONLY | O_CREAT | O_APPEND);
        if (laji_log_filefd == -1) {
            perror("laji_log_init()");
            return -1;
        }
        struct stat file_stat;
        fstat(laji_log_filefd, &file_stat);
        if (file_stat.st_size < 1024 * 1024 * 50) break;
    }
    return 0;
}

int laji_log(const char* buffer) {
    if (laji_log_enabled && laji_log_inited) {
        pthread_mutex_lock(&logger_mutex);

        // check filesize
        struct stat file_stat;
        fstat(laji_log_filefd, &file_stat);
        if (file_stat.st_size > 1024 * 1024 * 50) laji_log_findnewfile();

        // write
        char output_buffer[616], time_buffer[61];
        int output_bufferlen;
        time_t t;
    
        time(&t);
        strftime(time_buffer, 61, "%Y-%m-%d %H:%M:%S", localtime(&t)); 
        sprintf(output_buffer, "%s %c %s\n", time_buffer, 'V', buffer);
        output_bufferlen = strlen(output_buffer);
        //06-29 18:38:32.891   824   960 I ThermalEngine: Monitor : quiet_therm = 44, msm_therm = 47, ufs_therm = 44, battery_therm = 376,current_now = 26000
        write(laji_log_filefd, output_buffer, output_bufferlen);

        pthread_mutex_unlock(&logger_mutex);
        return 0;
    } else {
        if (!laji_log_inited && laji_log_enabled) {
            // warn user, should init
            // hey but this line is not thread safe.
            // but who cares?
            fputs("laji_log need initialize!\n", stdout);
            return -1;
        } else {
            // normally disabled the logger
            return 0;
        }
    }
}

int laji_log_close() {
    if (laji_log_filefd != -1 && laji_log_enabled && laji_log_inited) {
        close(laji_log_filefd);
    } else {
        // else ?
    }
}
