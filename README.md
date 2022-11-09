# meneameterm
Meneame.net for Linux Terminals
Classic Asteroid Game coded with SDL2 in C
* Remember to install libcurl and libcurl-dev libraries to compile.

TO COMPILE and RUN:  

    * GNU/LINUX:
    - Download or clone repository.
    - Type "bash build.sh" and ./meneame" to run.
    
![Alt text](men0.jpg?raw=true "Demo")

* Comment:

In my last project in C I wanted to bring together everything I've learned with graphical text interfaces and put it to use by connecting to the meneame API. I have been a user of meneame since its inception and this is my personal homage for being there for all these years.
For now it is a DEMO of what it could become. The program downloads the 100 news items that appear on the front page by making a CURL request to "https://meneame.net/api/list.php?status=published".

It then parses the received data and displays it in a program with a retro text interface.
Not my best or cleanest code intended to be used as a "demo". When you think you understand strings in C there is always a "Segmentation Fault" around the corner. But despite the countless "memory leaks" it is made with a lot of love :)

The explanation of all the modules below:

(Increasing in order of abstraction until you reach main.c)
________________

LEVEL 0: (BASE)
- rterm.c : With the ANSI escape sequences to color our output in the terminal
- tm.c : Create a millisecond timer for animations
- fileop.c: A module that helps with file operations
_______________

LEVEL 1: (DEPENDENCIES)
- download.c : Depends on libcurl. We make the call to the meneame API and save its content in a file
- parse.c : Parse the content of the file and dump it to an array. We remove the unicode sequences and parse Spanish accents (WEAKEST LINK/MEMORY LEAKS)
_______________

LEVEL 2: (LOW LEVEL ABSTRACTIONS)
- keyb.c : Creates a small keyboard abstraction to read escaped keys \e on Linux
- scbuf.c (1) : Creates an intermediate layer between the display and the terminal as a double screen buffer - listbox.c (2) : Creates a list with a selector with horizontal scroll
_______________

LEVEL 3: (HIGH LEVEL ABSTRACTIONS)
- ui.c : Different tools like textbox and window for user interface
- global.h and about.h : text messages and global variables
_______________

main.c: Main program

(1) http://oldstuff286.blogspot.com/2017/09/a-very-simple-screen-double-buffer.html

(2) http://oldstuff286.blogspot.com/2018/07/segmentation-fault-c-pointers.html
