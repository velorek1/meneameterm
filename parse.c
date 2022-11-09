/* MODULE TO PARSE A JSON OBJECT 
 * DOWNLOADED FROM MENEAME'S API
 * @author: velorek
 * Last modified: 6/11/2022
 TO-DO: FIX MEMORY LEAKS 
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "parse.h"
#include "global.h"
#include "fileop.h"

FILE *fichero=NULL;
char *bufferText;


NOTICIA noticias[100]={};


int instr(char *srcStr, char *searchStr,  long offset, int xorstr ){
  size_t s1len=0,s2len=0,i=0,a=0, pos=offset;
  int flag=-1;
  char *sword=NULL;
  s1len=strlen(srcStr);
  s2len=strlen(searchStr);
  sword = (char *)malloc((s2len+1)*sizeof(char));
  strcpy(sword,"\0");
   do{
    sword[a] = srcStr[i];
    a=a+1;
    i++;
    if (strcmp(sword,searchStr) == 0) {flag=0;pos = i-a; break;}
    if (a>=s2len) { a = 0; sword[s2len] = '\0'; pos++;i=pos;}
} while (i<s1len);
  if (sword != NULL) free(sword);
  if (xorstr==1) pos=pos+a; //position after string
  if (flag==-1) pos=-1; //error, string not found
  return pos;
}

int cutstr(char *srcStr, char *resStr , size_t initPos, size_t endPos ){
//remember to free string at the end
  size_t s1len=0,s2len=0,i=0,a=0, pos=0;
  char *tempStr=NULL;
  int flag=-1;
   s1len=strlen(srcStr);
   s2len=strlen(resStr);
  //printf("%ld:%ld:%ld:%ld\n", s1len,s2len,initPos,endPos);
  //if (s2len < (endPos-initPos)) return flag; //error destination is bigger than source
  if (endPos > s1len) endPos = s1len; //until the end of the string

  tempStr = (char *)malloc(((endPos-initPos)+1)*sizeof(char));
  strcpy(tempStr,"\0");
   do{
    tempStr[a] = srcStr[i+initPos];
   // printf("%c",resStr0[a]);
    a=a+1;
    i++;
    pos= i + initPos;
    } while (pos<endPos);
  tempStr[endPos - initPos] = '\0';
  strcpy(resStr,tempStr);
  if (tempStr != NULL) free(tempStr);
  if (flag==-1) pos=-1; //error
  return a; //length
}

void loadBuffer(){
  long    i = 0;
  long    byteCount = 0;
  char    ch;

  //Read char by char
   //open JSON file 
   openFile(&fichero,FICHERO1,"r"); 
   byteCount = getfileSize(FICHERO1);
   //try with backup file
   if (fichero == NULL || byteCount == 0){
      openFile(&fichero,FICHERO2,"r"); 
      byteCount = getfileSize(FICHERO2);
   }
   
   if (byteCount == 0 && fichero == NULL ) 
   { 
         printf("\r¡Error descargando el archivo! Compruebe la conexión e inténtelo más tarde. \n");
         exit(0);
   }
   
   bufferText = (char *) malloc(byteCount + 1);
   strcpy(bufferText, "\0");
   if(fichero != NULL) {
    rewind(fichero);	//Go to start of file
    ch = getc(fichero);	//peek into file
    while(!feof(fichero)) {
      //Read until SEPARATOR '.'
      bufferText[i] = ch;
      i++;
      ch = getc(fichero);
    }
  }

}

unsigned hextodec(char *hex)
{
    long long decimal = 0, base = 1;
    int i = 0, length;
    length = strlen(hex);
    for(i = length--; i >= 0; i--)
    {
        if(hex[i] >= '0' && hex[i] <= '9')
        {
            decimal += (hex[i] - 48) * base;
            base *= 16;
        }
        else if(hex[i] >= 'A' && hex[i] <= 'F')
        {
            decimal += (hex[i] - 55) * base;
            base *= 16;
        }
        else if(hex[i] >= 'a' && hex[i] <= 'f')
        {
            decimal += (hex[i] - 87) * base;
            base *= 16;
        }
    }
    return decimal;
}

int removeUnicode (char *newStr){
  long cut1=0, oldcut=0; long pos=0;
  int count=0;
  char nuevaString[255]={};
  int newlength=0;
  char uniString[7]={};
  char uni=0;
  int k=0; int c=0;
  pos=0;
 //remove unicode strings and convert them to spanish chars
  //nuevaString = (char *) malloc(strlen(newStr));
  strcpy(nuevaString, "\0");
  do{
    oldcut=cut1;
    cut1=instr(newStr, "\\u",pos,0);
    if (cut1==-1) break;
    else {
      
      for (k=pos; k<=cut1; k++) {
	     if (newStr[k] == '\\') break;
             nuevaString[c] = newStr[k];
	     c++;
      }
   
      if (oldcut!=cut1) count++; //we calculate how many uni characters we found
      cutstr(newStr, uniString , cut1+2, cut1+6);
      uni =hextodec(uniString);
      if (uni>0){
        nuevaString[c] = hextodec(uniString);
        c++;
      }
      pos=cut1+6;
    }
  } while (cut1 != -1);
    newlength = strlen(newStr);    
    for(k=pos; k<newlength; k++) {
	    nuevaString[c] = newStr[k];
	    c++;
    }
    nuevaString[c] ='\0'; //we close the string
    strcpy(newStr, nuevaString);
    //if (nuevaString != NULL) free (nuevaString);
    return count;
}


int removeUnicodeBig (char *newStr){
  long cut1=0, oldcut=0; long pos=0;
  int count=0;
  char nuevaString[4096]={};
  int newlength=0;
  char uniString[7];
  char uni=0;
  int k=0; int c=0;
  pos=0;
 //remove unicode strings and convert them to spanish chars
  //nuevaString = (char *) malloc(strlen(newStr));
  strcpy(nuevaString, "\0");
  do{
    oldcut=cut1;
    cut1=instr(newStr, "\\u",pos,0);
    if (cut1==-1) break;
    else {
      
      for (k=pos; k<=cut1; k++) {
	     if (newStr[k] == '\\') break;
             nuevaString[c] = newStr[k];
	     c++;
      }
   
      if (oldcut!=cut1) count++; //we calculate how many uni characters we found
      cutstr(newStr, uniString , cut1+2, cut1+6);
      uni =hextodec(uniString);
      if (uni>0){
        nuevaString[c] = hextodec(uniString);
        //printf("%d:%i\n", hextodec(uniString),nuevaString[c]);
        c++;
      }
      pos=cut1+6;
    }
  } while (cut1 != -1);
    newlength = strlen(newStr);    
    for(k=pos; k<newlength; k++) {
	    nuevaString[c] = newStr[k];
	    c++;
    }
    nuevaString[c] ='\0'; //we close the string
    strcpy(newStr, nuevaString);
    //if (nuevaString != NULL) free (nuevaString);
    return count;
}

long extractData(char *searchStr, char *endPattern, char *dumpStr, int delunicode, long *position)
{
  long cut1=0, cut2=0; 
  char newStr[255];
  //long size=0;
  //wchar_t uni;
    
    strcpy(newStr, "\0");
    cut1=instr(bufferText, searchStr,*position,0);
    if (cut1==-1) return cut1;
    cut2=instr(bufferText, endPattern,cut1+strlen(searchStr)+strlen(endPattern),0);
    //size=cut2-cut1;
    //newStr = (char *) malloc(size);
    //strcpy(newStr, "\0");
    cutstr(bufferText, newStr , cut1+strlen(searchStr)+strlen(endPattern), cut2);
    if (delunicode ==1) removeUnicode(newStr);
    strcpy(dumpStr, newStr);   
    *position=cut1;
    //count++;
    //if (newStr != NULL) free (newStr);
  return cut1;
}

long extractContent(char *searchStr, char *endPattern, char *dumpStr, int delunicode, long *position)
{
  long cut1=0, cut2=0; 
  char newStr[4096];
 // long size=0;
 // wchar_t uni;
    
    strcpy(newStr, "\0");
    cut1=instr(bufferText, searchStr,*position,0);
    if (cut1==-1) return cut1;
    cut2=instr(bufferText, endPattern,cut1+strlen(searchStr),0);
   // size=cut2-cut1;
    cutstr(bufferText, newStr , cut1+strlen(searchStr), cut2);
    if (delunicode ==1) removeUnicodeBig(newStr);
    strcpy(dumpStr, newStr);   
    *position=cut1;
  return cut1;
}


void creaArrayNoticias(){
  long pos2=0;
  char endquotes[4]={0,0,0,0};
  long ret=0;
  long k=0;
  setlocale(LC_ALL, "");
  
  loadBuffer();

//title
  pos2=0;
  endquotes[0] = 0x22;
  endquotes[1] = ',';
  endquotes[2] = 0x22;
  endquotes[3] = '\0';
  for (k=0; k<100; k++){
    ret=extractData("title", endquotes, noticias[k].title,1, &pos2);
    if (ret == -1) break;
  }

 
//clicks
  pos2=0;
  endquotes[0] = ',';
  endquotes[1] = 0x22;
  endquotes[2] = '\0';
  for (k=0; k<100; k++){
    ret=extractData("clicks", endquotes, noticias[k].clicks,0, &pos2);
    if (ret == -1) break;
  }

//votes
  pos2=0;
  endquotes[0] = ',';
  endquotes[1] = 0x22;
  endquotes[2] = '\0';
  for (k=0; k<100; k++){
    ret=extractData("votes", endquotes, noticias[k].votes,0, &pos2);
    if (ret == -1) break;
  }
//negatives
  pos2=0;
  endquotes[0] = ',';
  endquotes[1] = 0x22;
  endquotes[2] = '\0';
  for (k=0; k<100; k++){
    ret=extractData("negatives", endquotes, noticias[k].negatives,0, &pos2);
    if (ret == -1) break;
  }

//karma
  pos2=0;
  endquotes[0] = ',';
  endquotes[1] = 0x22;
  endquotes[2] = '\0';
  for (k=0; k<100; k++){
    ret=extractData("karma", endquotes, noticias[k].karma,0, &pos2);
    if (ret == -1) break;
  }

// content
  pos2=0;
  endquotes[0] = 0x22;
  endquotes[1] = ',';
  endquotes[2] = 0x22;
  endquotes[3] = '\0';
  
  for (k=0; k<100; k++){
    ret=extractContent("content\":\"", endquotes, noticias[k].content,1, &pos2);
    //Garbage collector: Remember to free 100 strings?
    if (ret == -1) break;
  } 

  if (bufferText != NULL) free (bufferText);
  if (fichero!=NULL) closeFile(fichero);
  
}




