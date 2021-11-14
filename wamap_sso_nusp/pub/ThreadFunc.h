
#ifndef BASE_THREAD_FUNC_H
#define BASE_THREAD_FUNC_H

#include <stdio.h>
#include <stdlib.h>


#ifdef _WINDOWS32

//#if defined(WAMAPDBLIB_EXPORTS)
//#define	IMEXPORT __declspec(dllexport)
//#else
//#define	IMEXPORT __declspec(dllimport)
//#endif

//#include <afx.h>
//#include <afxwin.h>         
//#include <afxext.h>         
//#include <process.h>

typedef DWORD (WINAPI *ThreadFunc)(void *);

//class IMEXPORT CThreadFuncs           
class CThreadFuncs           
{                           
public:
    static void Sleep(int millis) {::Sleep(millis);};
    static void startThread(ThreadFunc, void *param);
};

void CThreadFuncs::startThread(ThreadFunc func, void *param)
{
    HANDLE  tHandle;
    DWORD   threadID;

//     tHandle = CreateThread(0,          // Security Attributes,
//                            0x10000,    // Stack Size,
//                            func,       // Starting Address.
//                            param,      // Parmeters
//                            0,          // Creation Flags,
//                            &threadID); // Thread ID (Can not be null on 95/98)
	SECURITY_ATTRIBUTES attr;
	attr.nLength=sizeof(SECURITY_ATTRIBUTES);
	attr.lpSecurityDescriptor=NULL;
	attr.bInheritHandle=TRUE;
    tHandle = CreateThread(&attr,          // Security Attributes,
                           0,    // Stack Size,
                           (LPTHREAD_START_ROUTINE)func,       // Starting Address.
                           param,      // Parmeters
                           0,          // Creation Flags,
                           &threadID); // Thread ID (Can not be null on 95/98)

    if (tHandle == NULL)//0)
    {
        printf("<!!>Error starting Windowns thread.  Errno = %d\n", errno);
        exit(-1);
    }

    SetThreadPriority(tHandle, THREAD_PRIORITY_IDLE);
}


#elif defined (_AIX) || defined(_SUN) || defined(_LINUX) || defined(_HPUX) || defined (_ALPHA) || defined(_FREEBSD) || defined(__CYGWIN__)
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

//------------------------------------------------------------------------------
//
//   UNIX specific code for starting threads
//
//------------------------------------------------------------------------------

extern "C" {

typedef void (*ThreadFunc)(void *);
typedef void *(*pthreadfunc)(void *);

class CThreadFuncs           
{                           
public:
    static void Sleep(int millis);
    static void startThread(ThreadFunc, void *param);
};

void CThreadFuncs::Sleep(int millis)
{
   int seconds = millis/1000;
   if (seconds <= 0) seconds = 1;
#if defined(SOLARIS)
   // somehow the sleep hangs on Solaris
   // so ignore the call
#else
   ::sleep(seconds);
#endif
}


void CThreadFuncs::startThread(ThreadFunc func, void *param)
{
    unsigned long x;

    pthread_t tId;

#if defined(_HPUX)
    //x = pthread_create( &tId, pthread_attr_default,  (pthreadfunc)func,  param);
    x = pthread_create( &tId, NULL,  (pthreadfunc)func,  param);
#else
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    x = pthread_create( &tId, &attr,  (pthreadfunc)func,  param);
#endif
    if (x == -1)
    {
        printf("<!!>Error starting Unix thread.  Errno = %d\n", errno);
        exit(-1);
    }
}

}
#else
#error 当前操作系统平台不支持线程操作
#endif

#endif //BASE_THREAD_FUNC_H

