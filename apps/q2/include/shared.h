#ifndef __USERPROG__
#define __USERPROG__

#pragma once

#include "lab2-api.h"
#include "usertraps.h"

#define BUFFER_SIZE 32
#define PRODUCER_OBJ "producer.dlx.obj"
#define CONSUMER_OBJ "consumer.dlx.obj"

typedef struct circular_buffer {
  int head;
  int tail;
  char buffer[BUFFER_SIZE];
  lock_t lock;
} circular_buffer;

#endif
