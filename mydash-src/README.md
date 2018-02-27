/*! \mainpage README
 


## Project 3

* Author: Logan Barclay
* Class: CS453 [Operating Systems] Section #2

## Overview

A simple shell called mydash that handles normal command execution such as: ls, cd, etc..  The shell also has a simple jobs handler that keeps track of background jobs using a linked list data structure.  The jobs handler stores running background jobs in a list and reports running and fineshed jobs when prompted.

## Manifest
----------------------

|Filename       		  	  | Description                                                       |
|-----------------------------|-------------------------------------------------------------------|
|README.dox     		  	  |This file                                                          |
|doxygen-config 		  	  |Sample config file for doxygen                                     |
|Makefile       	  	  	  |build file for the mydash project 								  |
|TestCases      		  	  |An list of some of test cases I used                               |
|valgrind.supp  		  	  |Example suppression file for valgrind for the readline library     |
|mydash-src/Makefile 		  |Build file for mydash 	 					                 	  |
|mydash-src/mydash.c 		  |Main driver program 			                                  	  |
|mydash-src/error.c 		  |Error handling code used in class examples                    	  |
|mydash-src/mydash.h 		  |Header file for the mydash project                            	  |
|mydash-src/commands.c 		  |Class file that handles commands, adds jobs to the list 			  |
|mydash-src/Jobs.c 			  |Class file that handles job functions such as print 				  |
|mydash-src/parser.c 		  |Class file that parses the command line input args 				  |
|mydash-src/Makefile 		  |build file for the mydash-src folder 							  |
|other-progs/test-readline.c  |Example file on how to use auto completion with readline library   |
|other-progs/loop.c   		  |Simple infinite loop program for testing purposes            	  |
|other-progs/Makefile 		  |Makefile to build other program examples                     	  |

## Building the project

To build the program, run the "make" command while in the top level directory.  To run the program, use "./mydash".  Once the "mydash>" prompt is displayed, use normal shell commands to navigate directories, start processes, and display information.

## Features and usage

This simple shell utilizes the readline library and will auto complete directories and keep a history of commands.  Use th Tab key to auto-complete, and the up arrow to return the last commands.  Mydash will handle normal commands such as "ls", "cd", "exit", version "-v", as well as the emply command (enter key).  To start a background job, enter a command such as "sleep 5" followed by "&", such as "sleep 5 &".  This will start the background process and store it in the linked list.  To see jobs that have finished, press the enter key with no command.  To print a list of background jobs currently running, enter the command "jobs".

## Testing

Testing this code took the majority of my time on this project, and the reson why I am turning it in late.  While Valgrind was my main tool for testing things, a lot of it was trial and error.  I could not get gdb to work on my laptop for some reason so Valgrind was my main debugger for segfaults and such.  Starting slow with the basic commands like cd and ls, and them working up to the more complicated commands such as the background jobs with &.  I did not have unit testing, so I basically just entered different variations of commands to see if the behavior was normal.  I have included a small list of the cases I tested for.  This is just a fraction of the commands I tested, I should have been better about recording the various commands I used.

### Valgrind

Valgrind was the best and worst part about this project.  While it was incredibly frustrating trying to track down the memory leaks and errors, I became more Valgrind savy as time went on.  The reason I am turning this project in late was because I did not have a clean bill of health from Valgrind.  I fgured whatever cases you will run would exploit my memory leaks and invalid pointers, causing my program to crash and losing me more than 10% of my grade.  It was also very satisfying to see all those 0's on the valgrind screen!

I was basically using Valgrind the whole time I was entering test commands to check for memory leaks.  I would write down my series of commands and the results so I could reproduce the same commands and compare the output.  By doing this I was able to change small bits of code and see the effects.  I used the suppression file, as well as the track-origins=yes flag to find problems.  When I recieved the message with no memory leaks, I noticed I still had a lot of errors.  Using the -v flag I was able to correct these errors.  I now feel like my shell is fairly robust, but you probably have a way of breaking it!

### Known Bugs

I was not able to produce any bugs that I could track down.  Every once in a while a byte or two would show up in the still reachable section of Valgrind, but I could not reproduce it even with the same commands.  I figure it might be a race condition type of thing.

## Discussion

====================================================================================================

FIXING P1:
	1. No error message if CD fails
	To fix this I added the dirent.h library and used the diropen function to try to open the directory.  If it failed I would print an error message.
	2. -v command line argument did not exit the shell
	To fix this I just added an exit call instead of continuing the while loop.
	3. Parsing issues with not allowing space after '&'.
	To fix this I created a while loop to step back over the spaces at the end of the readline.
	4. Valgrind errors and leaks
	I fixed these by trial and error, I wasn't properly freeing "unmodline" causing either leaks or errors.  I was also trying to access a pointer I already freed in my printDoneJobs method.
	5. Doxygen issues
	I re-added the main page tag
	6. Organization
	I shortened my main method by moving my if statements into a new function in commands.c. I also moved the printJobs and printDoneJobs functions to the Jobs.c file because it made more sense to group all the jobs related calls into one file.

====================================================================================================

BASIC SIGNAL HANDLING:

The most difficult thig about adding the signal handling was figuring out where to place the ignores and the catches.  I just copied the code from example 3 in the signal exaples given to us.  I had to do some refactoring to my code to get it to work including creating an addJob() function.  Before I as just adding jobs directly in the exec code if they were a background job.  I know have a function so I can call it when a foreground job is stopped.  After I refactored, I had an issue where my output wasn't displaying properly.  This was because I was calling tokenEval() in each if statement, causing everything to be evaluated twice.  My next issue was that I wasn't displaying the stopped job properly.  This was because I was creating unmodline inside the if statement that creates background jobs.  I moved this outside of that statement, and had to move my frees around a bit to pass valgrind.  I did not have time to attempt the extra credit due to other major projects.

====================================================================================================

A lot of the issues I had at first were due to differences between my compiler and the one on Onyx.  Mostly the c99 initialization style.  I was able to complete the checkpoint fairly easy, but with memory leaks.  Fixing the memory leaks was the biggest problem I had.  I used the parser in our examples, and freeing the 2D malloc was a bit tricky to get right.  I decided to make a freeParsedList() function in my parser file that was a simple for loop to free tokens.  The next issue was figuring out where to use this free function.  I was freeing things too soon and causing big problems when my code went looking for something that was no longer there.  Creating and using the list was fairly straight forward after I read through the .h files.  I think my biggest issue was where to place the function calls.  I was placing things either too early or too late causing wierd output.

I took CS253 almost two years ago, and havent done any work in C since then.  I struggled with the os-warmup project, and it wasn't untill a few days ago that my brain started working in C again.  Pointers, malloc and free, etc. are pretty tough when youre used to Java.


## Sources used

I hacked this together with snippets from the CS453 examples, the CS253 examples, and the GNU C library site.

