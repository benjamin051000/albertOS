/*
 * G8RTOS_Scheduler.h
 */
#pragma once

#include <albertOS.h>

using threadID = uint32_t;
using TaskFuncPtr = void(*)(); // Function pointer alias

const unsigned MAX_THREADS = 25;
const unsigned MAX_PTHREADS = 6;

const unsigned STACKSIZE = 512;
const unsigned OSINT_PRIORITY = 7;
const unsigned MAX_NAME_LEN = 16;


enum sched_ErrCode {
    NO_ERROR                  = 0,
    THREAD_LIMIT_REACHED      = -1,
    NO_THREADS_SCHEDULED      = -2,
    THREADS_INCORRECTLY_ALIVE = -3,
    THREAD_DOES_NOT_EXIST     = -4,
    CANNOT_KILL_LAST_THREAD   = -5,
    IRQn_INVALID              = -6,
    HWI_PRIORITY_INVALID      = -7
};

/* Holds the current time for the whole System */
extern uint32_t systemTime;

/**
 * API namespace
 */
namespace albertOS {
/*
 * Initializes variables and hardware for G8RTOS usage
 */
void init();

/*
 * Starts G8RTOS Scheduler
 * 	- Initializes Systick Timer
 * 	- Sets Context to first thread
 * Returns: Error Code for starting scheduler. This will only return if the scheduler fails
 */
sched_ErrCode launch();

/*
 * Adds threads to G8RTOS Scheduler
 * 	- Checks if there are stil available threads to insert to scheduler
 * 	- Initializes the thread control block for the provided thread
 * 	- Initializes the stack for the provided thread
 * 	- Sets up the next and previous tcb pointers in a round robin fashion
 * Param "threadToAdd": Void-Void Function to add as preemptable main thread
 * Returns: Error code for adding threads
 */
sched_ErrCode addThread(TaskFuncPtr threadToAdd, uint8_t priorityLevel, char name[MAX_NAME_LEN]);

/*
 * Adds periodic threads to G8RTOS Scheduler
 * Function will initialize a periodic event struct to represent event.
 * The struct will be added to a linked list of periodic events
 * Param Pthread To Add: void-void function for P thread handler
 * Param period: period of P thread to add
 * Returns: Error code for adding threads
 */
sched_ErrCode addPeriodicEvent(TaskFuncPtr PthreadToAdd, uint32_t period);


/*
 * Puts the current thread into a sleep state.
 *  param durationMS: Duration of sleep time in ms
 */
void sleep(uint32_t durationMS);

//returns the currently running thread's ID
threadID getThreadID();

sched_ErrCode killThread(threadID threadID);

//kills currently running thread
sched_ErrCode killSelf();

//kills all threads except currently running
sched_ErrCode killAll();

//adds an aperiodic event, like an interrupt
sched_ErrCode addAPeriodicEvent(TaskFuncPtr AthreadToAdd, uint8_t priority, IRQn_Type IRQn);

} // end of namespace albertOS
