#ifndef __USERPROG__
#define __USERPROG__

#pragma once

#include "lab2-api.h"
#include "usertraps.h"

#define BUFFER_SIZE 3
#define PRODUCER_OBJ "producer.dlx.obj"
#define CONSUMER_OBJ "consumer.dlx.obj"

typedef struct circular_buffer {
  int head;
  int tail;
  char buffer[BUFFER_SIZE];
  lock_t lock;
  sem_t s_fullslots;
  sem_t s_emptyslots;
} circular_buffer;

#endif
