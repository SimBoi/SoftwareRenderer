/*****************************************************************************
* Irit_prll.h - control parallelization of irit.			     *
*									     *
* Written by:  Gershon Elber				Ver 0.1, Apr. 2019   *
******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                *
******************************************************************************
* Control over the  parallelization of irit.				     *
*****************************************************************************/

#ifndef	IRIT_PARALLEL_H
#define	IRIT_PARALLEL_H

#ifdef IRIT_OPEN_MP
#include <omp.h>
#endif /* IRIT_OPEN_MP */

IRIT_GLOBAL_DATA_HEADER int
    _IritParallelExec,
    _IritParallelMaxThreads;

/* #define DEBUG_STATE_MODIFIED_REPORT_ALL */

/* Parallel execution controls: */
#ifdef IRIT_COMPILE_PARALLEL
    #ifdef DEBUG
	#define DEBUG_STATE_MODIFIED_REPORT_PRLL
        #define IRIT_PARALLEL_NOT_REENTRANT_CODE_ALERT()  { \
	    fprintf(stderr, "Warning: [%s:%d] invocation of non reentrant code\n", \
	            __FILE__, __LINE__); \
        }
    #else
        #define IRIT_PARALLEL_NOT_REENTRANT_CODE_ALERT()
    #endif /* DEBUG */

    #ifdef __WINNT__
	#define IRIT_MUTEX		         VoidPtr
	#define IRIT_MUTEX_GEN_VAR(IritMutex) \
					IRIT_STATIC_DATA IRIT_MUTEX IritMutex;
	#define IRIT_MUTEX_LOCK(IritMutex) \
				      IritEmulatePthreadMutexLock(&IritMutex);
	#define IRIT_MUTEX_UNLOCK(IritMutex) \
				     IritEmulatePthreadMutexUnLock(IritMutex);
	#define IRIT_PARALLEL_LOOP_NO_PARAMS() omp parallel for
	#define IRIT_PARALLEL_LOOP_WITH_PARAMS(Private, Shared) \
					omp parallel for Private Shared
    #else
	#include <pthread.h>
	#define IRIT_MUTEX			 pthread_mutex_t
	#define IRIT_MUTEX_GEN_VAR(IritMutex) \
					IRIT_STATIC_DATA IRIT_MUTEX IritMutex;
	#define IRIT_MUTEX_LOCK(IritMutex)   pthread_mutex_lock(&IritMutex);
	#define IRIT_MUTEX_UNLOCK(IritMutex) pthread_mutex_unlock(&IritMutex);
	#define IRIT_PARALLEL_LOOP_NO_PARAMS() omp parallel for
	#define IRIT_PARALLEL_LOOP_WITH_PARAMS(Private, Shared) \
					omp parallel for Private Shared
    #endif /* __WINNT__ */

    #define IRIT_PARALLEL_IS_PARALLEL_NOW() (omp_get_num_threads() > 1)
    #define IRIT_PARALLEL_LOCKED_CODE(Mutex, code) \
					IRIT_MUTEX_LOCK(Mutex);  \
					code; \
					IRIT_MUTEX_UNLOCK(Mutex);
#else
    #define IRIT_PARALLEL_NOT_REENTRANT_CODE_ALERT()
    #define IRIT_MUTEX VoidPtr
    #define IRIT_MUTEX_GEN_VAR(IritMutex)
    #define IRIT_MUTEX_LOCK(IritMutex)
    #define IRIT_MUTEX_UNLOCK(IritMutex)
    #define IRIT_PARALLEL_LOOP_NO_PARAMS()
    #define IRIT_PARALLEL_LOOP(Private, Shared)
    #define IRIT_PARALLEL_IS_PARALLEL_NOW() (0)
    #define IRIT_PARALLEL_LOCKED_CODE(Mutex, code) code;
#endif /* IRIT_COMPILE_PARALLEL */

#ifdef DEBUG_STATE_MODIFIED_REPORT_PRLL
    /* Reports if a thread other than the master thread changes a state. */
    #define IRIT_DEBUG_STATE_CHAR_MODIFIER_INVOKED(Val) { \
        if (omp_get_thread_num() > 0) \
            fprintf(stderr, "Warning [%d]: char state modified at %s, %d - \"%s\" value = %d (%c)\n", \
		    omp_get_thread_num(), __FILE__, __LINE__, #Val, Val, Val); \
        }
    #define IRIT_DEBUG_STATE_INT_MODIFIER_INVOKED(Val) { \
        if (omp_get_thread_num() > 0) \
            fprintf(stderr, "Warning [%d]: int state modified at %s, %d - \"%s\" value = %d\n", \
		    omp_get_thread_num(), __FILE__, __LINE__, #Val, Val); \
        }
    #define IRIT_DEBUG_STATE_REAL_MODIFIER_INVOKED(Val) { \
        if (omp_get_thread_num() > 0) \
	    fprintf(stderr, "Warning [%d]: real state modified at %s, %d - \"%s\" value = %f\n", \
		    omp_get_thread_num(), __FILE__, __LINE__, #Val, Val); \
        }
    #define IRIT_DEBUG_STATE_PTR_MODIFIER_INVOKED(Val) { \
        if (omp_get_thread_num() > 0) \
	    fprintf(stderr, "Warning [%d]: Ptr state modified at %s, %d - \"%s\" value = %08lx\n", \
		    omp_get_thread_num(), __FILE__, __LINE__, #Val, ((long unsigned int) ((IritIntPtrSizeType) Val))); \
        }
#elif defined(DEBUG_STATE_MODIFIED_REPORT_ALL)
    #define IRIT_DEBUG_STATE_CHAR_MODIFIER_INVOKED(Val) { \
        fprintf(stderr, "Warning: char state modified at %s, %d - \"%s\" value = %d (%c)\n", \
		__FILE__, __LINE__, #Val, Val, Val); \
	}
    #define IRIT_DEBUG_STATE_INT_MODIFIER_INVOKED(Val) { \
	fprintf(stderr, "Warning: int state modified at %s, %d - \"%s\" value = %d\n", \
		__FILE__, __LINE__, #Val, Val); \
	}
    #define IRIT_DEBUG_STATE_REAL_MODIFIER_INVOKED(Val) { \
	fprintf(stderr, "Warning: real state modified at %s, %d - \"%s\" value = %f\n", \
		__FILE__, __LINE__, #Val, Val); \
	}
    #define IRIT_DEBUG_STATE_PTR_MODIFIER_INVOKED(Val) { \
	fprintf(stderr, "Warning: Ptr state modified at %s, %d - \"%s\" value = %08lx\n", \
	        __FILE__, __LINE__, #Val, ((long unsigned int) ((IritIntPtrSizeType) Val))); \
	}
#else
    #define IRIT_DEBUG_STATE_CHAR_MODIFIER_INVOKED(Val)
    #define IRIT_DEBUG_STATE_INT_MODIFIER_INVOKED(Val)
    #define IRIT_DEBUG_STATE_REAL_MODIFIER_INVOKED(Val)
    #define IRIT_DEBUG_STATE_PTR_MODIFIER_INVOKED(Val)
#endif /* DEBUG */

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

int IritSetIritParallelExec(int Prll);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* IRIT_PARALLEL_H */
