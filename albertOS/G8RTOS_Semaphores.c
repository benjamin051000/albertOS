    /*
 * G8RTOS_Semaphores.c
 */

/*********************************************** Dependencies and Externs *************************************************************/

#include <G8RTOS.h>

/*********************************************** Dependencies and Externs *************************************************************/


/*********************************************** Public Functions *********************************************************************/

/*
 * Initializes a semaphore to a given value
 * Param "s": Pointer to semaphore
 * Param "value": Value to initialize semaphore to
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_InitSemaphore(Semaphore* s, int32_t value)
{
	int32_t status = StartCriticalSection();

	(*s) = value;

	EndCriticalSection(status);
}

/*
 * No longer waits for semaphore
 *  - Decrements semaphore
 *  - Blocks thread is sempahore is unavalible
 * Param "s": Pointer to semaphore to wait on
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_WaitSemaphore(Semaphore* s)
{
    int32_t status = StartCriticalSection();

    (*s) -= 1; //declare ownership

    if((*s) < 0)
    {
        CurrentlyRunningThread->blocked = s; //block thread
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; //yield to allow other threads to run
    }

    EndCriticalSection(status);
}

/*
 * Signals the completion of the usage of a semaphore
 *  - Increments the semaphore value by 1
 *  - Unblocks any threads waiting on that semaphore
 * Param "s": Pointer to semaphore to be signaled
 * THIS IS A CRITICAL SECTION
 */
void G8RTOS_SignalSemaphore(Semaphore* s)
{
    int32_t status = StartCriticalSection();

    (*s) += 1;

    if((*s) <= 0)
    {
        tcb_t* ptr = CurrentlyRunningThread->nextTCB;
        while(ptr->blocked != s) //unblock first thread associated with this semaphore
        {
            ptr = ptr->nextTCB;
        }

        *(ptr->blocked) = 0; //make it unblocked
    }

    EndCriticalSection(status);
}

/*********************************************** Public Functions *********************************************************************/


