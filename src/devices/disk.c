#include "devices/disk.h"
#include "devices/timer.h"
#include <stdio.h>
#include <stdlib.h>

static struct list disk_queue;
static struct lock disk_lock;

void disk_init(void) {
    list_init(&disk_queue);
    lock_init(&disk_lock);
}

void disk_schedule_request(int sector, bool is_write, void *data, int deadline) {
    struct disk_request *req = malloc(sizeof(struct disk_request));
    req->sector = sector;
    req->is_write = is_write;
    req->data = data;
    req->deadline = deadline;

    lock_acquire(&disk_lock);
    list_insert_ordered(&disk_queue, &req->elem, disk_request_compare, NULL);
    lock_release(&disk_lock);
}

static bool disk_request_compare(const struct list_elem *a, const struct list_elem *b, void *aux UNUSED) {
    const struct disk_request *req_a = list_entry(a, struct disk_request, elem);
    const struct disk_request *req_b = list_entry(b, struct disk_request, elem);
    return req_a->deadline < req_b->deadline;
}

void disk_process(void) {
    while (true) {
        struct disk_request *req = NULL;

        lock_acquire(&disk_lock);
        if (!list_empty(&disk_queue)) {
            req = list_entry(list_pop_front(&disk_queue), struct disk_request, elem);
            lock_release(&disk_lock);

            // Simulate disk read/write operation
            printf("Processing disk %s at sector %d\n", req->is_write ? "write" : "read", req->sector);
            timer_sleep(100);

            free(req);
        } else {
            lock_release(&disk_lock);
            timer_sleep(10);
        }
    }
}
