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
 * Stats of the scheduler simulator run, airthmentic means at the top level, 
 * individual threads at the second level. 
 * 
 * For example, in a mildly JSON like syntax, stats for a run with two threads:
 * 
 *  {
 *    Mean waiting time  
 *    Mean turnaround time
 *    thread count = 2
 *    [  
 *      { 
 *        tid of T1
 *        Waiting time for T1
 *        Turnaround time for T1
 *      },
 *      { 
 *        tid of T2
 *        Waiting time for T2
 *        Turnaround time for T2
 *      }
 *    ]
 *  }
 * 
 * You create a stats object like so:
 * 
 *   stats_t *stats = malloc(sizeof(stats_t));
 *   stats->thread_count = < whatever >;
 *   stats->tstats = malloc(sizeof(stats_t)*stats->thread_count);
 * 
 * Then fill in the individual stats->tstats[0..thread_count-1] records for 
 * each thread and set the mean waiting and turnaround times on stats itself.
 */
typedef struct __stats_t {
  unsigned int tid;           // Only meaningful at second level
  unsigned int waiting_time; 
  unsigned int turnaround_time;
  unsigned int thread_count;  // Number of elements in TSTATS
  struct __stats_t *tstats;   // Address of the first of THREAD_COUNT stats_t 'objects'
} stats_t;
