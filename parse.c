/* MODULE TO PARSE A JSON OBJECT 
 * DOWNLOADED FROM MENEAME'S API
 * @author: velorek
 * Last modified: 7/01/2023 - FIXED MEMORY LEAKS! 
Example of structure being parsed:
{"id":3766755,"permalink":"https:\/\/www.meneame.net\/m\/cultura\/marco-pantani-muerte-ciclista?utm_source=meneame_api","go":"https:\/\/www.meneame.net\/m\/cultura\/marco-pantani-muerte-ciclista?utm_source=meneame_api","url":"https:\/\/www.meneame.net\/m\/cultura\/marco-pantani-muerte-ciclista?utm_source=meneame_api","uri":"marco-pantani-muerte-ciclista","from":"sport.jotdown.es","sub":"cultura","status":"published","user":"salvattore.padre","clicks":331,"votes":38,"negatives":0,"karma":485,"comments":2,"title":"Marco Pantani, muerte de un ciclista","tags":"pantani, drogas, doping, mafia","sent_date":1673087049,"voted":null,"sponsored":false,"date":1673110520,"content":"La Comisi\u00f3n Antimafia acaba de admitir anomal\u00edas en su exclusi\u00f3n al Giro. \u00abEs necesario seguir las investigaciones\u00bb, asevera. \"Alguien le mat\u00f3 para llevarse la pasta, porque por esa cantidad de coca no se puede morir. Sobre todo, porque Pantani la fumaba. No la esnifaba, fumaba crack y en la habitaci\u00f3n no hab\u00eda botellas de agua, papel platino o bicarbonato\" (...) \"Pantani fue procesado por fraude. Luego resbal\u00f3 en el agujero oscuro de la coca\u00edna, el sexo desenfrenado, la depresi\u00f3n. Muri\u00f3 rodeado de gente, pero en realidad estaba solo\"","content_type":"text","source":"https:\/\/sport.jotdown.es\/2023\/01\/06\/pantani-muerte-de-un-ciclista\/","thumb":"https:\/\/www.meneame.net\/backend\/media?type=link&id=3766755"}
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "parse.h"
#include "global.h"
#include "fileop.h"

FILE *fichero=NULL;
char *bufferText=NULL;


NOTICIA noticias[100]={};


int instr(char *srcStr, char *searchStr,  long offset, int xorstr ){
  size_t s1len=0,s2len=0,i=0,a=0, pos=offset;
  int flag=-1;
  char *sword=NULL;
  if (srcStr == NULL || searchStr == NULL) return -1; 
  s1len=strlen(srcStr); 
  s2len=strlen(searchStr);
  sword = (char *)malloc((s2len+1)*sizeof(char));
  strcpy(sword,"");
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
   strcpy(resStr,"");
   s1len=strlen(srcStr);
   s2len=strlen(resStr);
  //printf("%ld:%ld:%ld:%ld\n", s1len,s2len,initPos,endPos);
  //if (s2len < (endPos-initPos)) return flag; //error destination is bigger than source
  if (endPos > s1len) endPos = s1len; //until the end of the string

  tempStr = (char *)malloc(((endPos-initPos)+1)*sizeof(char));
  strcpy(tempStr,"");
 
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

void removeUnicode(char *input_string, char *output_string) {
    int input_length = strlen(input_string);
    int output_index = 0;

    for (int i = 0; i < input_length; i++) {
        // Check if the current character is a backslash
        if (input_string[i] == '\\') {
            // Check if the next character is a 'u'
            if (input_string[i + 1] == 'u') {
                // Convert the unicode character to a regular character
                char unicode_char[5];
                unicode_char[0] = input_string[i + 2];
                unicode_char[1] = input_string[i + 3];
                unicode_char[2] = input_string[i + 4];
                unicode_char[3] = input_string[i + 5];
                unicode_char[4] = '\0';

                int unicode_int = (int)strtol(unicode_char, NULL, 16);
                char regular_char = (char)unicode_int;

                output_string[output_index] = regular_char;
                output_index++;

                // Skip the next 4 characters (\uXXXX)
                i += 5;
            } else {
                // The backslash is not followed by a 'u', so just copy it to the output string
                output_string[output_index] = input_string[i];
                output_index++;
            }
        } else {
            // The current character is not a backslash, so just copy it to the output string
            output_string[output_index] = input_string[i];
            output_index++;
        }
    }
    output_string[output_index] = '\0'; // null-terminate the output string
}



long extractData(char *tempBfr, char *searchStr, char *endPattern, char *dumpStr, int delunicode, long *position)
{
  long cut1=0, cut2=0; 
  char newStr[255];
  long size=0;
  //wchar_t uni;
    
    strcpy(newStr, "");
    strcpy(dumpStr, "");

    cut1=instr(tempBfr, searchStr,*position,0);
    if (cut1==-1) return cut1;
    cut2=instr(tempBfr, endPattern,cut1+strlen(searchStr)+strlen(endPattern),0);
    size=cut2-cut1;
    //Protect memory crashes
    if (size>=255) {strcpy(dumpStr, "String too long!"); *position=cut1; return cut1;}
    //newStr = (char *) malloc(size);
    cutstr(tempBfr, newStr , cut1+strlen(searchStr)+strlen(endPattern), cut2);
    if (delunicode ==1) removeUnicode(newStr,dumpStr);
    else strcpy(dumpStr, newStr);   
    *position=cut1;
    //count++;
    //if (newStr != NULL) free (newStr);
  return cut1;
}

long extractContent(char *tempBfr, char *searchStr, char *endPattern, char *dumpStr, int delunicode, long *position)
{
  long cut1=0, cut2=0; 
  char newStr[4096];
  long size=0;
 // wchar_t uni;
    
    strcpy(newStr, "");
    strcpy(dumpStr, "");
    cut1=instr(tempBfr, searchStr,*position,0);
    if (cut1==-1) {strcpy(dumpStr, "Story could not be read!"); return cut1;}
    cut2=instr(tempBfr, endPattern,cut1+strlen(searchStr),0);
    size=cut2-cut1;
    //Protect memory crashes
    if (size>=4096) {strcpy(dumpStr, "String too long!"); *position=cut1; return cut1;}
    cutstr(tempBfr, newStr , cut1+strlen(searchStr), cut2);
    if (delunicode ==1) removeUnicode(newStr,dumpStr);
    else strcpy(dumpStr, newStr);   
    *position=cut1;
  return cut1;
}

void creaArrayNoticias(){
  long pos2=0;
  char endquotes[4]={0,0,0,0};
  long ret=0;
  long k=0;
  setlocale(LC_ALL, "");
  
//loadBuffer, opens file and dumps into memory to be parsed and split into different sections
  long    i = 0;
  long    byteCount = 0;
  char    ch=0;

  //Read char by char
   //open JSON file 
   openFile(&fichero,FICHERO1,"r"); 
   byteCount = getfileSize(FICHERO1);
   if (byteCount == 0) {
     fprintf(stderr, "Failed to download file to create list.\n");
     exit(0);
   }
   if (byteCount < 14000){
     fprintf(stderr, "MENEAME API has not responded as expected. Resorting to demo file.\n");
     closeFile(fichero);
     openFile(&fichero,DEMOFILE,"r"); 
     byteCount = getfileSize(DEMOFILE);
   }
   //try with backup file
   if (fichero == NULL || byteCount == 0){
      closeFile(fichero);
      openFile(&fichero,FICHERO2,"r"); 
      byteCount = getfileSize(FICHERO2);
   }
   
   if (byteCount == 0 && fichero == NULL ) 
   { 
         printf("\r¡Error descargando el archivo! Compruebe la conexión e inténtelo más tarde. \n");
         exit(0);
   }
   
   bufferText = (char *) malloc((byteCount + 1) * sizeof(char));
   strcpy(bufferText, "");
   
   if(fichero != NULL) {
    rewind(fichero);	//Go to start of file
    ch = getc(fichero);	//peek into file
    strcpy(bufferText, "");
    i=0;
    while(!feof(fichero)) {
      bufferText[i] = ch;
      i++;
      ch = getc(fichero);
    }
    bufferText[i]= '\0';
  }

//Parse data
//title
  pos2=0;
  strcpy(endquotes, "");
  endquotes[0] = 0x22;
  endquotes[1] = ',';
  endquotes[2] = 0x22;
  endquotes[3] = '\0';
  for (k=0; k<100; k++){
    strcpy(noticias[k].title, "");
    ret=extractData(bufferText, "title", endquotes, noticias[k].title,1, &pos2);
    if (ret == -1) break;
  }

 
//clicks
  pos2=0;
  endquotes[0] = ',';
  endquotes[1] = 0x22;
  endquotes[2] = '\0';
  for (k=0; k<100; k++){
    strcpy(noticias[k].clicks, "");
    ret=extractData(bufferText, "clicks", endquotes, noticias[k].clicks,0, &pos2);
    if (ret == -1) break;
  }

//votes
  pos2=0;
  endquotes[0] = ',';
  endquotes[1] = 0x22;
  endquotes[2] = '\0';
  for (k=0; k<100; k++){
    strcpy(noticias[k].votes, "");
    ret=extractData(bufferText, "votes", endquotes, noticias[k].votes,0, &pos2);
    if (ret == -1) break;
  }
//negatives
  pos2=0;
  endquotes[0] = ',';
  endquotes[1] = 0x22;
  endquotes[2] = '\0';
  for (k=0; k<100; k++){
    strcpy(noticias[k].negatives, "");
    ret=extractData(bufferText, "negatives", endquotes, noticias[k].negatives,0, &pos2);
    if (ret == -1) break;
  }

//karma
  pos2=0;
  endquotes[0] = ',';
  endquotes[1] = 0x22;
  endquotes[2] = '\0';
  for (k=0; k<100; k++){
    strcpy(noticias[k].karma, "");
    ret=extractData(bufferText, "karma", endquotes, noticias[k].karma,0, &pos2);
    if (ret == -1) break;
  }

// content
  pos2=0;
  endquotes[0] = 0x22;
  endquotes[1] = ',';
  endquotes[2] = 0x22;
  endquotes[3] = '\0';
  
  for (k=0; k<100; k++){
    //strcpy(noticias[k].content, "");
    ret=extractContent(bufferText, "content\":\"", endquotes, noticias[k].content,1, &pos2);
    //Garbage collector: Remember to free 100 strings?
    if (ret == -1) break;
  } 

  free (bufferText);
  closeFile(fichero);
  
}




