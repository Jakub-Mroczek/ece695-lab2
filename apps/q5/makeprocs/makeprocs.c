#pragma once

#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "shared.h"

void main (int argc, char *argv[])
{
  int molecule_types = 5;
  int num_n3 = 0;
  int num_h2o = 0;
  int num_n = 0;
  int num_h2 = 0;
  int num_o2 = 0;
  int num_no2 = 0;
  int i;                          // Loop index variable
  atoms *a;                       // Used to get address of shared memory page
  uint32 h_mem;                   // Used to hold handle to shared memory page
  sem_t s_procs_completed;        // Semaphore used to wait until all spawned processes have completed
  char h_mem_str[10];             // Used as command-line argument to pass mem_handle to new processes
  char s_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes
  sem_t s_n3;
  sem_t s_h2o;
  sem_t s_n;
  sem_t s_h2;
  sem_t s_o2;
  sem_t s_no2;

  Printf("Q5 running\n");

  if (argc != 3) {
    Printf("Usage: "); Printf(argv[0]); Printf(" <number of N3 molecules> <number of H2O molecules>\n");
    Exit();
  }

  // Convert string from ascii command line argument to integer number
  num_n3 = dstrtol(argv[1], NULL, 10); // the "10" means base 10
  Printf("Injecting %d N3 molecules\n", num_n3);

  num_h2o = dstrtol(argv[2], NULL, 10); // the "10" means base 10
  Printf("Injecting %d H2O molecules\n", num_h2o);

  // Allocate space for a shared memory page, which is exactly 64KB
  // Note that it doesn't matter how much memory we actually need: we 
  // always get 64KB
  if ((h_mem = shmget()) == 0) {
    Printf("ERROR: could not allocate shared memory page in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

  // Map shared memory page into this process's memory space
  if ((a = (atoms *)shmat(h_mem)) == NULL) {
    Printf("Could not map the shared page to virtual address in "); Printf(argv[0]); Printf(", exiting..\n");
    Exit();
  }

  s_n3 = sem_create(0);
  s_h2o = sem_create(0);
  s_n = sem_create(0);
  s_h2 = sem_create(0);
  s_o2 = sem_create(0);
  s_no2 = sem_create(0);

  a->num_n3 = num_n3;
  a->num_h2o = num_h2o;
  a->num_n = num_n3*3;        // 3 N atoms per N3 molecule
  a->num_h2 = (num_h2o/2)*2;  // 2 H2 molecues per 2 H2O molecules
  a->num_o2 = (num_h2o/2);
  a->num_no2 = (a->num_n < a->num_o2) ? a->num_n : a->num_o2; // min(num_n, num_o2)

  a->s_n3 = s_n3;
  a->s_h2o = s_h2o;  
  a->s_n = s_n;
  a->s_h2 = s_h2; 
  a->s_o2 = s_o2;
  a->s_no2 = s_no2; 

  // Create semaphore to not exit this process until all other processes 
  // have signalled that they are complete.
  if ((s_procs_completed = sem_create(-((molecule_types)-1))) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  // Setup the command-line arguments for the new process.  We're going to
  // pass the handles to the shared memory page and the semaphore as strings
  // on the command line, so we must first convert them from ints to strings.
  ditoa(h_mem, h_mem_str);
  ditoa(s_procs_completed, s_procs_completed_str);

  process_create(N3_INJECT_OBJ, h_mem_str, s_procs_completed_str, NULL);
  process_create(H2O_INJECT_OBJ, h_mem_str, s_procs_completed_str, NULL);
  process_create(REACTION_1_OBJ, h_mem_str, s_procs_completed_str, NULL);
  process_create(REACTION_2_OBJ, h_mem_str, s_procs_completed_str, NULL);
  process_create(REACTION_3_OBJ, h_mem_str, s_procs_completed_str, NULL);

  // And finally, wait until all spawned processes have finished.
  if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf("\n");
    Exit();
  }
  Printf("All other processes completed, exiting main process.\n");
  Printf("Q5 END\n");
}
