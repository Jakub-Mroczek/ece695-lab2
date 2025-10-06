#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "shared.h"

void main (int argc, char *argv[]){
    int i;
    int j;
    atoms *a;
    uint32 h_mem;
    sem_t s_procs_completed;

    if (argc != 3) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
    } 

    // Convert the command-line strings into integers for use as handles
    h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
    s_procs_completed = dstrtol(argv[2], NULL, 10);

    // Map shared memory page into this process's memory space
    if ((a = (atoms *)shmat(h_mem)) == NULL) {
        Printf("Could not map the virtual address to the memory in "); Printf(argv[0]); Printf(", exiting...\n");
        Exit();
    }

    // Now print a message to show that everything worked
    // Printf("reaction_1: reaction_1 with PID %d created\n", Getpid());

    i = 0;
    while(i < a->num_n3) {
        if(sem_wait(a->s_n3) != SYNC_SUCCESS) {
            Printf("Bad sem_wait(s_n3)"); Printf(", exiting...\n");
            Exit();
        }
        for(j = 0; j < 3; j++) { // each N3 molecule produces 3 N atoms
            if(sem_signal(a->s_n) != SYNC_SUCCESS) {
                Printf("Bad sem_signal(s_n)"); Printf(", exiting...\n");
                Exit();
            }
            else {
               Printf("An N molecule is created\n");
            }
        }
        i++;
    }

    // Printf("reaction_1: reaction_1 with PID %d is complete\n", Getpid());
    if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
        Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
        Exit();
    }

}