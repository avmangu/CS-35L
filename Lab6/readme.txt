NAME: Animesh Mangu
UID: 804-771-005
TA: Guangyu Zhou

Homework 6 Action-Report

In this homework, we modified the ray tracer code and parallelized it by
multithreading the file main.c and adding the -lpthread option to the
Makefile.

Before our changes to main.c, the algorithm iterated through every pixel in the
developed image, generated the coordinate values (x, y), and then outputted the
values of these colors. To achieve this, the prior version used multiple nested
loops which repetitively ran this function on a number of pixels. This was the
feature which I needed to modify in order to multithread and speed up the code.

The first thing I did was port this functionality from the main method into a
seperate method called colorEncode, with a parameter to the number of
threads. The reason for this was because to multithread the code, we had to use
the pthread library. The pthread function accepted arguments for one function to
parallelize, and thus the the functionality we needed to multithreaded was
exported into colorEncode. In the colorEncode, I basically turned the image into
a 2D array of pixels, and used the "divide-and-conquer" algorithm to iterate and
fill up the array using seperate threads.

In the main method, we used pthread_create and pthread_join to implement the
multithreading functionality. Once the iteration was complete and the threads
finished the task of filling up the 2D array, we could simply use a nested
loop to print all the values of the rows and columns. The only issue I ran into
was a minor race condition since my program never ended and was stuck on
1-test.ppm forever. I didn't know whether it was taking  too long or there was
an error, so I traced through the code. Upon doing this, I realized the issue
was simply a reinitialized variable which had been ported to global in the new
version, while the old version was left in the main. This was fixed, and to
confirm that our program worked, I used the diff command to byte-by-byte compare
1-test.ppm, 2-test.ppm, 4-test.ppm, and 8-test.ppm with the bas line.ppm. The
results were positive. 

When the command make clean check was run to compare the timing / speed of the
code with a different number of threads each time (1, 2, 4, 8), the one with
8 threads was the fastest, followed by 4, 2, and then 1. This was as expected, since
the higher number of threads allows us to paralize our code more
efficiently. However, this is not always the case, as using TOO many threads
(like 25) can actually hurt the performance of the program by overloading the
thread stack size, maxing the virtual memory, etc. However, as seen in our case,
the performance improved tremendously (reulsts shown in make-log.txt), as the number of
threads increased from 1 to 8.

The END!

