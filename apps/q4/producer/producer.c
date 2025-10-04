#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "shared.h"

void main (int argc, char *argv[]){
    circular_buffer *cb;
    uint32 h_mem;
    sem_t s_procs_completed;
    char payload[] = PAYLOAD;
    int i;

    if (argc != 3) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
    } 

    // Convert the command-line strings into integers for use as handles
    h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
    s_procs_completed = dstrtol(argv[2], NULL, 10);

    // Map shared memory page into this process's memory space
    if ((cb = (circular_buffer *)shmat(h_mem)) == NULL) {
        Printf("Could not map the virtual address to the memory in "); Printf(argv[0]); Printf(", exiting...\n");
        Exit();
    }

    // Now print a message to show that everything worked
    Printf("producer: Producer with PID %d created\n", Getpid());

    i = 0;
    while (payload[i] != '\0') {
        //produce item
        char c = payload[i];

        if(lock_acquire(cb->lock) != SYNC_SUCCESS) {
            Printf("Bad lock_acquire(cb->lock)"); Printf(", exiting...\n");
            Exit();
        }
        while (((cb->head + 1) % BUFFER_SIZE == cb->tail)) {
        //buffer is full
            if(cond_wait(cb->cond_not_full) != SYNC_SUCCESS) {
                Printf("Bad cond_wait(cb->cond_not_full)"); Printf(", exiting...\n");
                Exit();
            }
        }
        cb->buffer[cb->head] = c;
        cb->head = (cb->head+ 1) % BUFFER_SIZE;
        Printf("Producer %d inserted: %c\n", Getpid(), c);

        if(cond_signal(cb->cond_not_empty) != SYNC_SUCCESS) {
            Printf("Bad cond_signal(cb->cond_not_empty)"); Printf(", exiting...\n");
            Exit();
        }
        if(lock_release(cb->lock) != SYNC_SUCCESS) {
            Printf("Bad lock_release(cb->lock)"); Printf(", exiting...\n");
            Exit();
        }
        i++;
    }

    Printf("producer: Producer with PID %d is complete\n", Getpid());
    if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
        Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
        Exit();
    }

}