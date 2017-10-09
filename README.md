# Matrix multiplication example pthreads using MS Visual Studio 2017

Instructions for implementing pthreads on MS Visual Studio 2017:
* Download: http://cs.du.edu/~mitchell/pthreads_compiled.zip
* add: pthread.h, sched.h & semaphore.h to "C:\Program Files (x86)\Windows Kits\10\Include\10.0.15063.0\ucrt"
* add: pthreadVC2.lib to "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.15063.0\ucrt\x86"
* add: pthreadVC2.dll to "C:\Windows"
* add: pthreadVC2.lib to the Project Properties > Configuration Properties > Linker > Input > Additional Dependencies
* add: #define _TIMESPEC_DEFINED 
