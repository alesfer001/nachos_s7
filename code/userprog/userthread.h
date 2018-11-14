#ifndef __USER_THREAD__
#define __USER_THREAD__

#include "system.h"
#include "syscall.h"
#include "synch.h"

struct threadArgs
{
  int func;
  int arg;
};

static Semaphore* lock_nbThreads = new Semaphore("lock threads counter", 1);

extern int do_ThreadCreate(int f, int arg);

extern int do_ThreadExit();

static void StartUserThread(void * schmurtz);

#endif
