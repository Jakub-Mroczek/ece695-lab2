#pragma once

#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "shared.h"

void main (int argc, char *argv[])
{
  int numpairs = 0;               // Used to store number of processes to create
  int i;                          // Loop index variable
  circular_buffer *cb;            // Used to get address of shared memory page
  uint32 h_mem;                   // Used to hold handle to shared memory page
  sem_t s_procs_completed;        // Semaphore used to wait until all spawned processes have completed
  char h_mem_str[10];             // Used as command-line argument to pass mem_handle to new processes
  char s_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes
  lock_t lock;

  Printf("Q2 running\n");

  if (argc != 2) {
    Printf("Usage: "); Printf(argv[0]); Printf(" <number of producers and consumers to create>\n");
    Exit();
  }

  // Convert string from ascii command line argument to integer number
  numpairs = dstrtol(argv[1], NULL, 10); // the "10" means base 10
  Printf("Creating %d producers and consumers\n", numpairs);

  // Allocate space for a shared memory page, which is exactly 64KB
  // Note that it doesn't matter how much memory we actually need: we 
  // always get 64KB
  if ((h_mem = shmget()) == 0) {
    Printf("ERROR: could not allocate shared memory page in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

  // Map shared memory page into this process's memory space
  if ((cb = (circular_buffer *)shmat(h_mem)) == NULL) {
    Printf("Could not map the shared page to virtual address in "); Printf(argv[0]); Printf(", exiting..\n");
    Exit();
  }

  // Put some values in the shared memory, to be read by other processes
  if ((lock = lock_create()) == SYNC_FAIL) {
    Printf("Bad lock_create lock in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  cb->head = 0;
  cb->tail = 0;
  cb->lock = lock;

  // Create semaphore to not exit this process until all other processes 
  // have signalled that they are complete.
  if ((s_procs_completed = sem_create(-((numpairs*2)-1))) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  // Setup the command-line arguments for the new process.  We're going to
  // pass the handles to the shared memory page and the semaphore as strings
  // on the command line, so we must first convert them from ints to strings.
  ditoa(h_mem, h_mem_str);
  ditoa(s_procs_completed, s_procs_completed_str);

  // Now we can create the processes.  Note that you MUST end your call to
  // process_create with a NULL argument so that the operating system
  // knows how many arguments you are sending.
  for(i=0; i<numpairs; i++) {
    process_create(PRODUCER_OBJ, h_mem_str, s_procs_completed_str, NULL);
    // Printf("Producer %d created\n", i);
    process_create(CONSUMER_OBJ, h_mem_str, s_procs_completed_str, NULL);
    // Printf("Consumer %d created\n", i);
  }

  // And finally, wait until all spawned processes have finished.
  if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf("\n");
    Exit();
  }
  Printf("All other processes completed, exiting main process.\n");
  Printf("Q2 END\n");
}
