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

/*********************************************************
 **               DO NOT MODIFY THIS FILE               ** 
 *********************************************************/

#pragma once

/**
 * Default is First Come First Served, only Round Robin has a quantum.
 */
enum algorithm {
  FIRST_COME_FIRST_SERVED,
  ROUND_ROBIN,
  NON_PREEMPTIVE_PRIORITY,
  PREEMPTIVE_PRIORITY,
  NON_PREEMPTIVE_SHORTEST_JOB_FIRST,
  PREEMPTIVE_SHORTEST_JOB_FIRST,
  NON_PREEMPTIVE_SHORTEST_REMAINING_TIME_FIRST,
  PREEMPTIVE_SHORTEST_REMAINING_TIME_FIRST
};

/**
 * If you want enriched threads in a list (and you probably will at some 
 * point) wrap them inside some other structure with additional attributes. 
 * Do NOT modify this structure.
 */
typedef struct __thread_t {
  unsigned int tid;      // Thread ID 
  unsigned int priority; // 0 = highest - only relevant to priority schedulers
  unsigned int length;   // Only relevant to SJF and SRTF schedulers
} thread_t;

/**
 * Returns the current simulator time in abstract units equal to one programable 
 * clock cycle.
 */
int sim_time();

/**
 * Have the built-in dispatcher run a thread on the CPU by performing a context 
 * switch if necessary. 
 * 
 * If a context switch is necessary, the built-in dispatcher takes exacly one 
 * simulator clock cycle to execute. If a context switch is not necessary 
 * (because the CPU is idle and the thread is new) dispatching is instantaneous.
 * 
 * (1) Any new thread scheduled without spending time on the ready queue will 
 *     be dispatched in the clock tick it was scheduled in.
 * 
 * (2) Any new thread scheduled after spending at least one clock tick on the ready 
 *     queue will be dispatched in the clock tick after the one it was scheduled in.
 * 
 * (3) Any thread scheduled after an IO operation completes will be dispatched in 
 *     the clock tick after the one it was scheduled in.
 * 
 * Examples:
 * 
 * (a) Thread T arrives in Tick 2 and the CPU is idle 
 *     dispatch(T) called in Tick 2
 *        T will start executing in Tick 2
 * 
 * (b) Thread T arrives in Tick 4 and the CPU is busy
 *     CPU becomes available in Tick 8
 *     dispatch(T) called in Tick 8
 *        T will start executing in Tick 9
 * 
 * (c) Thread T completes an IO operation in Tick 12 and the CPU is idle
 *     dispatch(T) called in Tick 12
 *        T will start executing in Tick 13
 * 
 * (d) Thread T completes an IO operation in Tick 18 but the CPU is busy
 *     CPU becomes available in Tick 22
 *     dispatch(T) called in Tick 22
 *        T will start executing in Tick 23
 */
void sim_dispatch(thread_t *t);
