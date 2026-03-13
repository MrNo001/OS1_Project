# OS1_Project
Project for the course OperatingSystems1 at the University of Belgrade

Notes: Za debug pre nego sto se postavi prekidna rutina ne koristiti sistemski poziv

The provided __putc() doesn't relly on any kernel code.It doesn't need the console handler or any other support from the environment.

The Thread and Semaphore classes shouldnt be instantiated in global scope becouse the compiler wants to to call an automatic heap-destructor and can't;


Make sure not to call new in global scope.