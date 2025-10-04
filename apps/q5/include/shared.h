#ifndef __USERPROG__
#define __USERPROG__

#pragma once

#include "lab2-api.h"
#include "usertraps.h"

#define N3_INJECT_OBJ "n3_inject.dlx.obj"
#define H2O_INJECT_OBJ "h2o_inject.dlx.obj"
#define REACTION_1_OBJ "reaction_1.dlx.obj"
#define REACTION_2_OBJ "reaction_2.dlx.obj"
#define REACTION_3_OBJ "reaction_3.dlx.obj"

typedef struct atoms {
  int num_n3;
  int num_h2o;
  sem_t s_n3;
  sem_t s_h2o;
  sem_t s_n;
  sem_t s_h2;
  sem_t s_o2;
  sem_t s_no2;
} atoms;

#endif
