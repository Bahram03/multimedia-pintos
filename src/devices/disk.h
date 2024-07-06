#ifndef THREADS_DISK_H
#define THREADS_DISK_H

#include <list.h>
#include "threads/synch.h"

struct disk_request {
    int sector;
    bool is_write;
    void *data;
    int deadline;
    struct list_elem elem;
};

void disk_init(void);
void disk_schedule_request(int sector, bool is_write, void *data, int deadline);
void disk_process(void);

#endif
