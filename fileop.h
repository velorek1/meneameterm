/*
========================================================================
- HEADER -
Meneame for Terminals
Module with different file functions
@author : Velorek
@version : 1.0
Last modified: 05/01/2022
========================================================================
*/

#ifndef _FILEOP_H_
#define _FILEOP_H_

#include <stdio.h>

int closeFile(FILE * fileHandler);
int openFile(FILE ** fileHandler, char *fileName, char *mode);
long getfileSize(char *fileName);
int file_exists(char *fileName);
long copyFile(char *fileSource, char *fileDestination);
#endif


