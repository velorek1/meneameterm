/*
========================================================================
Meneame for Terminals
Module with different file functions
@author : Velorek
@version : 1.0
Last modified: 05/01/2022
========================================================================
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int closeFile(FILE * fileHandler) {
  int     ok;
  ok = fclose(fileHandler);
  return ok;
}

int openFile(FILE ** fileHandler, char *fileName, char *mode) {
  int     ok;
  *fileHandler = fopen(fileName, mode);
  //check whether buffer is assigned
  //and return value
  if(*fileHandler != NULL)
    ok = 1;
  else
    ok = 0;
  return ok;
}

long getfileSize(char *fileName) {
  long    sz=0;
  FILE *file1=NULL;
  openFile(&file1, fileName, "rb");
  rewind(file1);
  if(file1 != NULL) {
    fseek(file1, 0L, SEEK_END);
    sz = ftell(file1);
    rewind(file1);
  }

  closeFile(file1);
  return sz;
}

long copyFile(char *fileSource, char *fileDestination){
int res=0;
FILE *file1=NULL, *file2=NULL;
long    byteCount = 0;
char    ch;
res = openFile(&file1, fileSource, "rb");
openFile(&file2, fileDestination, "wb");
if (res != 0) {
  //Read char by char
    printf("here2");
    rewind(file1);	//Go to start of file
    fread(&ch, sizeof(ch), 1, file1);	// Peek into file
    while(!feof(file1)) {
      byteCount += fwrite(&ch, sizeof(ch), 1, file2);	// Write data to file
      fread(&ch, sizeof(ch), 1, file1);
    }
  }
closeFile(file1);
closeFile(file2);
return byteCount;
}	

/*----------------------*/
/* Check if file exists */
/*----------------------*/

int file_exists(char *fileName) {
  int     ok;
  if(access(fileName, F_OK) != -1) {
    ok = 1;         //File exists
  } else {
    ok = 0;
  }
  return ok;
}

