/************************************************************************
 *
 * CSE130 Assignment 4
 *
 * Copyright (C) 2021-2022 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 * 
 ************************************************************************/

/**
 * See scheduler.h for function details. All are callbacks; i.e. the simulator 
 * calls you when something interesting happens.
 */
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "simulator.h"
#include "scheduler.h"

typedef struct new_t {
  thread_t *t;
  int tid;
  int total_wait;
  int waiting_time;
  int start_time;
  int arrival;
  int finish;
  int tick_count;
  int remaining_time;
} new_t;

static void *ready_queue;
static void *stats_queue; //to use after evrything else is done
static thread_t *running = NULL; //to keep track of running thread
enum algorithm algo;
int quant;

static bool inner_equalitor(void *outer, void *inner)
{
  return ((new_t*)outer)->tid == ((thread_t*)inner)->tid;
}

static int inner_comparator(void *a, void *b)
{
  return ((thread_t*)a)->priority - ((thread_t*)b)->priority;
}

static int inner_comparator_length(void *a, void *b)
{
  return ((thread_t*)a)->length - ((thread_t*)b)->length;
}

/**
 * Initalise a scheduler implemeting the requested ALGORITHM. QUANTUM is only
 * meaningful for ROUND_ROBIN.
 */
void scheduler(enum algorithm algorithm, unsigned int quantum) {
  //create ready queue
  ready_queue = queue_create();
  stats_queue = queue_create();
  algo = algorithm;
  quant = quantum;
}

/**
 * Thread T is ready to be scheduled for the first time.
 */
void sys_exec(thread_t *t) {
  queue_enqueue(ready_queue, t); 
  new_t *new = calloc(1, sizeof(new_t));
  new->tid = t->tid;
  new->arrival = sim_time();
  new->start_time = sim_time();
  new->waiting_time = 0;
  new->tick_count = 0;
  new->remaining_time = 0;
  queue_enqueue(stats_queue, new);
}

/**
 * Programmable clock interrupt handler. Call sim_time() to find out
 * what tick this is. Called after all calls to sys_exec() for this
 * tick have been made.
 */
void tick() { 
  //NON_PREEMPTIVE_PRIORITY
  if(queue_size(ready_queue) && algo == NON_PREEMPTIVE_PRIORITY)
  {
    queue_sort(ready_queue, inner_comparator);
  }

  //NON_PREEMPTIVE_SHORTEST_JOB_FIRST
  if(queue_size(ready_queue) && algo == NON_PREEMPTIVE_SHORTEST_JOB_FIRST)
  {
    queue_sort(ready_queue, inner_comparator_length);
  }

  //NON_PREEMPTIVE_SHORTEST_REMAINING_TIME_FIRST
  if(queue_size(ready_queue) && algo == NON_PREEMPTIVE_SHORTEST_REMAINING_TIME_FIRST)
  {
    queue_sort(ready_queue, inner_comparator_length);
    //TODO
  }

  //PREEMPTIVE_PRIORITY
  //based on time - need to put running back to RQ if another thread with greater priority comes in 
  //get the next thread, compare priority
  if(queue_size(ready_queue) && algo == PREEMPTIVE_PRIORITY)
  {
    queue_sort(ready_queue, inner_comparator);
    thread_t *t = queue_head(ready_queue); 
    if(running && running->priority > t->priority)
    {
      new_t *t1 = queue_find(stats_queue, inner_equalitor, running);
      new_t *t2 = queue_find(stats_queue, inner_equalitor, t);
      //store waiting time for running thread and set time for next thread going into cpu
      t2->waiting_time += sim_time() - t2->start_time;
      t1->start_time = sim_time();
      queue_enqueue(ready_queue, running);
      queue_dequeue(ready_queue);
      sim_dispatch(t);
      running = t;
    }
  }

  //PREEMPTIVE_SHORTEST_JOB_FIRST
  //same as preemptive with length
  if(queue_size(ready_queue) && algo == PREEMPTIVE_SHORTEST_JOB_FIRST)
  {
    queue_sort(ready_queue, inner_comparator_length);
    thread_t *t = queue_head(ready_queue); 
    if(running && running->length > t->length)
    {
      new_t *t1 = queue_find(stats_queue, inner_equalitor, running);
      new_t *t2 = queue_find(stats_queue, inner_equalitor, t);
      //store waiting time for running thread and set time for next thread going into cpu
      t2->waiting_time += sim_time() - t2->start_time;
      t1->start_time = sim_time();
      queue_enqueue(ready_queue, running);
      queue_dequeue(ready_queue);
      sim_dispatch(t);
      running = t;
    }
  }

  //PREEMPTIVE_SHORTEST_REMAINING_TIME_FIRST
  //TODO

  //ROUND_ROBIN
  new_t *newt = NULL;
  if(running)
  {
    newt = queue_find(stats_queue, inner_equalitor, running);
  }
  //if thread has spent quant ticks on cpu then kick it out and put in RQ
  if(newt && newt->tick_count + 1 >= quant && algo == ROUND_ROBIN)
  {
    queue_enqueue(ready_queue, running);
    newt->start_time = sim_time();
    running = NULL;
  }
  //if nothing running, then take the next thread
  else if(queue_size(ready_queue) && algo == ROUND_ROBIN && !running)
  {
    thread_t *t1 = queue_dequeue(ready_queue);
    new_t *new1 = queue_find(stats_queue, inner_equalitor, t1);
    new1->waiting_time += sim_time() - new1->start_time;
    new1->tick_count = 0;
    sim_dispatch(t1);
    running = t1;
  }
  //increment tick count with every cpu tick
  if(newt && running)
  {
    newt->tick_count++;
  }

  //FCFS
  //if something is already running, do not dispatch
  //dispatch head of ready list if at least one ready thread
  if(queue_size(ready_queue) && !running)
  {
    thread_t *t = queue_dequeue(ready_queue);
    new_t *new1 = queue_find(stats_queue, inner_equalitor, t);
    new1->waiting_time += sim_time() - new1->start_time;
    new1->tick_count = 0;
    sim_dispatch(t);
    running = t;
  }
}

/**
 * Thread T has completed execution and should never again be scheduled.
 */
void sys_exit(thread_t *t) {
  new_t *new = queue_find(stats_queue, inner_equalitor, t); //to store in stats queue
  new->finish = sim_time() + 1; 
  running = NULL; //clear running thread indicator
}

/**
 * Thread T has requested a read operation and is now in an I/O wait queue.
 * When the read operation starts, io_starting(T) will be called, when the
 * read operation completes, io_complete(T) will be called.
 */
void sys_read(thread_t *t) {
  new_t *new = queue_find(stats_queue, inner_equalitor, t);   //to store in stats queue
  new->start_time = sim_time()+1;  //to store started wait time
  running = NULL;  //clear running thread indicator
}


/**
 * Thread T has requested a write operation and is now in an I/O wait queue.
 * When the write operation starts, io_starting(T) will be called, when the
 * write operation completes, io_complete(T) will be called.
 */
void sys_write(thread_t *t) {
  sys_read(t); //to get rid of duplication
}

/**
 * An I/O operation requested by T has completed; T is now ready to be 
 * scheduled again.
 */
void io_complete(thread_t *t) {
  new_t *new = queue_find(stats_queue, inner_equalitor, t);   //to store in stats queue
  new->start_time = sim_time()+1;
  queue_enqueue(ready_queue, t);  //put it back to RQ when completed io
}

/**
 * An I/O operation requested by T is starting; T will not be ready for
 * scheduling until the operation completes.
 */
void io_starting(thread_t *t) {
  new_t *new = queue_find(stats_queue, inner_equalitor, t);  //to store in stats queue
  new->waiting_time += sim_time() - new->start_time;  //need to calculate the waiting time when in io
}

/**
 * Return dynamically allocated stats for the scheduler simulation, see 
 * scheduler.h for details. Memory allocated by your code will be free'd
 * by the similulator. Do NOT return a pointer to a stack variable.
 */
stats_t *stats() {
  stats_t *stats = calloc(1, sizeof(stats_t));
  //go through the queue to get the number of threads
  stats->thread_count = queue_size(stats_queue);
  stats->tstats = calloc(1, sizeof(stats_t)*stats->thread_count);
  
  int total_wait = 0;
  int turnaround_time = 0;
  //loop to store the time for each thread
  for(int i = 0; i < stats->thread_count; i++)
  {
    new_t *t = queue_dequeue(stats_queue);
    stats->tstats[i].tid = t->tid;
    stats->tstats[i].waiting_time = t->waiting_time;
    stats->tstats[i].turnaround_time = t->finish - t->arrival;
    total_wait += stats->tstats[i].waiting_time;
    turnaround_time += stats->tstats[i].turnaround_time;
    free(t);
  }

  //average times
  stats->waiting_time = total_wait / stats->thread_count;
  stats->turnaround_time = turnaround_time /stats->thread_count;
  stats->thread_count = stats->thread_count;

  return stats;
}
