/*
========================================================================
- HEADER -
Módulo para parsear un objeto JSON en strings.
@author : Velorek
@version : 1.0
Last modified: 31/10/2022
========================================================================
*/

#ifndef _PARSE_H_
#define _PARSE_H_
#include <stdio.h>

typedef struct _noticia{
   int id;
   //char *permalink;
   //char *go;
   //char *url;
   //char *from;
   //char *status;
   //char *user;
   char clicks[255];
   char votes[255];
   char negatives[255]; 
   char karma[255];
   int comments;
   char title[255];
   //char *tags;
   //char *dates;
   char content[4096];
   //char *content_type;
   //char *source;
   char thumb;
} NOTICIA;


void creaArrayNoticias();
int instr(char *srcStr, char *searchStr,  long offset, int xorstr );
int cutstr(char *srcStr, char *resStr , size_t initPos, size_t endPos);

#endif


