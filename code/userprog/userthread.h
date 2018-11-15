#ifndef __USER_THREAD__
#define __USER_THREAD__

#include "system.h"
#include "syscall.h"
#include "synch.h"

#define MAXSEM 10

struct threadArgs
{
  int func;
  int arg;
  int exit_value;
};

static Semaphore* lock_nbThreads = new Semaphore("lock threads counter", 1);

//static Semaphore* sems;

extern int do_ThreadCreate(int f, int arg, int exit_value);

extern int do_ThreadExit();

static void StartUserThread(void * schmurtz);

#endif
