#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define DATA_SIZE 1000000 

long long timespec_diff_ns(struct timespec *start, struct timespec *end) {
    return (end->tv_sec - start->tv_sec) * 1000000000LL + (end->tv_nsec - start->tv_nsec);
}

int main() {
    int* data = malloc(DATA_SIZE * sizeof(int));
    if (data == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Measure time taken to write to memory
    struct timespec start_memory, end_memory;
    clock_gettime(CLOCK_MONOTONIC, &start_memory);
    for (int i = 0; i < DATA_SIZE; i++) {
        data[i] = i;
    }
    clock_gettime(CLOCK_MONOTONIC, &end_memory);
    long long time_memory = timespec_diff_ns(&start_memory, &end_memory);
    printf("Time taken to write to memory: %lld nanoseconds\n", time_memory);

    // Measure time taken to write to disk
    struct timespec start_disk, end_disk;
    clock_gettime(CLOCK_MONOTONIC, &start_disk);
    // Simulate writing to disk by opening a file and writing data to it
    int fd = open("data.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("Error opening file\n");
        return 1;
    }
    for (int i = 0; i < DATA_SIZE; i++) {
        if (write(fd, &data[i], sizeof(int)) != sizeof(int)) {
            printf("Error writing to file\n");
            close(fd);
            return 1;
        }
    }
    if (fsync(fd) == -1) {
        printf("Error syncing file\n");
        close(fd);
        return 1;
    }
    close(fd);
    clock_gettime(CLOCK_MONOTONIC, &end_disk);

    long long time_disk = timespec_diff_ns(&start_disk, &end_disk);
    printf("Time taken to write to disk: %lld nanoseconds\n", time_disk);

    free(data);

    return 0;
}

