#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "shared.h"

void main (int argc, char *argv[]){
    circular_buffer *cb;
    uint32 h_mem;
    sem_t s_procs_completed;
    char payload[] = "0123456789";
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
    Printf("consumer: Consumer with PID %d created\n", Getpid());


    for (i = 0; payload[i] != '\0'; i++) {
        //item to consume
        char c = payload[i];

        int flag = 0;
        while(flag == 0) {
            lock_acquire(cb->lock);
            if (!(cb->head == cb->tail)) {
            // buffer is not empty
                if(cb->buffer[cb->tail] == c) {
                // buffer has char were seeking
                    cb->buffer[cb->tail] = '\0';
                    cb->tail = (cb->tail+ 1) % BUFFER_SIZE;
                    flag = 1;
                    Printf("Consumer %d removed: %c\n", Getpid(), c);
                }          
            }
            lock_release(cb->lock);
        }
    }

    Printf("consumer: Consumer with PID %d is complete\n", Getpid());
    if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
        Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
        Exit();
    }

}