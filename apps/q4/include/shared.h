#ifndef __USERPROG__
#define __USERPROG__

#pragma once

#include "lab2-api.h"
#include "usertraps.h"

#define PAYLOAD_LEN 10 // "0123456789"
#define PAYLOAD "0123456789"
#define PRODUCER_OBJ "producer.dlx.obj"
#define CONSUMER_OBJ "consumer.dlx.obj"

typedef struct circular_buffer {
  int head;
  int tail;
  char buffer[BUFFERSIZE];
  lock_t lock;
  cond_t cond_not_empty;
  cond_t cond_not_full;
} circular_buffer;

#endif
