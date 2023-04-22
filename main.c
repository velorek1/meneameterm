/*
 * MENEAME FOR TERMINALS
 * Compile with -funsigned-char for Spanish accents / unicode compatibility 
 * @author: Velorek
 * @date: 6/11/2022
*/

#include <stdio.h>
#include "scbuf.h"
#include "rterm.h"
#include "listbox.h"
#include "tm.h"
#include "ui.h"
#include "download.h"
#include "parse.h"
#include "keyb.h"
#include "global.h"
#include "about.h"
#define STATUSMSG1 "^v NAVEGAR LISTA | F1: AYUDA | i: IR A | ESC: SALIR\0"
#define STATUSMSG2 "INTRO: VER NOTICIA | RETROCESO: VOLVER | CTRL-C: SALIR\0"
#define STATUSMSG3  "ESC: VOLVER\0"

char texto[MAX_TEXT]; 
//Prototypes
void draw_screen();
int tick();
char list();
int status=0;

// GLOBALS : SCREEN1 : ANIMATED | SCREEN2 : BASE SCREEN
LISTCHOICE *listBox1 = NULL;
SCREENCELL *screen1=NULL;
SCREENCELL *screen2=NULL;
NTIMER timer1;
SCROLLDATA scrollData;
int d_rows=0, d_columns=0;
int statuslimit=0;
int newsc_rows=0, newsc_columns=0;
int timer_speed = TIMER_SPEED;
int globaltic=0;
void draw_screen(){
//BASE SCREEN IS STORED IN SCREEN 2
int i=0;
char ch=0;

     if (screen1 != NULL) deleteList(&screen1);
     //Init 2 : Create 2 Screens for a double buffer  approach  
     d_rows=newsc_rows;
     d_columns=newsc_columns;
     create_screen(&screen1);
     //SCREEN 2
     screen_color(screen1, B_WHITE, F_WHITE, 0x20);
     //Top-Bar
     for (i=52;i<d_columns;i++){
          write_ch(screen1, i,1, 0x20, B_YELLOW, F_WHITE,FALSE);
          write_ch(screen1, i,2, 0x20, B_BLACK, F_WHITE,FALSE);
     }
     for (i=0;i<d_columns;i++) write_ch(screen1, i,d_rows, 0x20, B_YELLOW, F_WHITE,FALSE);
     //write_str(screen1,3,1,TITLEMSG1, B_YELLOW, F_BLACK,FALSE);
     window(screen1,1,1,52,7,B_YELLOW,F_WHITE,B_WHITE,0,1,1);
     for (int i=0; i<6; i++){
         write_str(screen1,2,i+1,about_msg[i], B_YELLOW, F_BLACK,FALSE);
     }
     for (i=0;i<strlen(STATUSMSG1);i++){
	     statuslimit=54+i;
        if (statuslimit <d_columns) write_ch(screen1,statuslimit,2,STATUSMSG1[i], B_BLACK, F_WHITE,FALSE);
     }
     window(screen1,4,9,d_columns-4,d_rows-3,B_WHITE,F_BLACK,B_BLACK,1,0,1);
     write_str(screen1,1,d_rows,"-> 2022 v0.1 | CODED By v3l0r3k", B_YELLOW, F_BLACK,FALSE);        
     
     //window(screen1,d_columns-46,3,d_columns-18,6,B_WHITE,F_BLACK,B_BLACK,1,0,1);
     //textbox(screen1, d_columns-44,4,15, "Buscar: ", texto, B_WHITE,F_BLACK, F_BLACK,FALSE,NOT_LOCKED);
     //draw news to screen (mock) careful with non-printable chars
     int inc=0;
     for (i=0;i<d_rows-13;i++) {
	    inc=0;
         for (int k=0; k<d_columns-12; k++) {
		    if (k<strlen(noticias[i].title)){
		       if(noticias[i].title[k]>31) {
			       write_ch(screen1, k+7-inc,10+i, noticias[i].title[k], B_WHITE, F_BLACK,FALSE);
		        }
		       else
			inc++;
		    }
                 }
   	 }
}

int tick(void){
char unstr[28] = "[+] MENEAME PARA TERMINALES\0";
     get_terminal_dimensions(&newsc_rows,&newsc_columns);
     outputcolor(F_BLACK, B_YELLOW);
     for (int i=0;i<strlen(unstr); i++){
        gotoxy(54+i,1);
       if (54+i<=d_columns) printf("%c", unstr[i]);
     }
     outputcolor(F_WHITE,B_YELLOW);
     gotoxy(54+globaltic,1);
     printf("%c\n",unstr[globaltic]);
     globaltic++;
     if (globaltic ==28) globaltic=0;
     if (newsc_rows != d_rows || newsc_columns != d_columns) return -1;
}

void clean_bars(){
     for (int i=53;i<d_columns;i++){
          write_ch(screen1, i,2, 0x20, B_BLACK, F_WHITE,FALSE);
     }
 update_screen(screen1);
}

char list() {
int i=0;
char ch=0;
  clean_bars();
  for (i=0;i<strlen(STATUSMSG2);i++){
	     statuslimit=54+i;
        if (statuslimit <d_columns) write_ch(screen1,statuslimit,2,STATUSMSG2[i], B_BLACK, F_WHITE,FALSE);
   }
  
  //write_str(screen1,54,2,"INTRO: VER NOTICIA | RETROCESO: VOLVER | CTRL-C: SALIR ", B_BLACK, F_WHITE,FALSE);
  //write_str(screen1,53,1,"[+] MENEAME PARA TERMINALES ", B_YELLOW, F_WHITE,TRUE);
  dump_screen(screen1);
  if (listBox1 != NULL) ch = listBox(listBox1, 8, 10, &scrollData, B_WHITE, F_BLACK, B_BLACK,F_WHITE, d_rows-7-6, LOCKED);
  
  clean_bars();
     for (i=0;i<strlen(STATUSMSG1);i++){
	     statuslimit=54+i;
        if (statuslimit <d_columns) write_ch(screen1,statuslimit,2,STATUSMSG1[i], B_BLACK, F_WHITE,FALSE);
     } 
  //write_str(screen1,54,2,"^v NAVEGAR LISTA | F1: AYUDA | G: IR A | ESC: SALIR", B_BLACK, F_WHITE,FALSE);
  return ch;
}
void addItems(LISTCHOICE ** listBox1) {
//Load items into the list.  
  //if (*listBox1 != NULL) removeList(listBox1);
  char ch=0;
  for (int h=0; h<100; h++)
      *listBox1 = addatend(*listBox1, newitem(noticias[h].title));

}
void display_story(int story){
char titlestr[15];
char lineStr[255];
int strPtr=0;
char ch=0;
int k=0;
  sprintf(titlestr,"Noticia #%d:", story+1);
  if (screen2 != NULL) deleteList(&screen2);
  create_screen(&screen2);
  copy_screen(screen2,screen1);
  clean_bars();
  write_str(screen1,54,2,STATUSMSG3, B_BLACK, F_WHITE,FALSE);
  window(screen1,(d_columns/2)-20,(d_rows/2)-5,(d_columns/2) + 20,(d_rows/2)+5,B_BLACK,F_WHITE,B_BLACK,1,0,0);
  dump_screen(screen1);
  listBox1= NULL; 
  removeList(&listBox1);
  scrollData.displayMetrics=0;
  listBox1 = addatend(listBox1, newitem(titlestr));
  strcpy(lineStr,"\0");
  strPtr=0;
  do{
    for (k=0; k<39; k++){
	  if (k==0 && noticias[story].content[strPtr] == 0x20) strPtr++;
	  lineStr[k]=noticias[story].content[strPtr];
	  strPtr++;
    }
  
   // make sure we don't split words
   if (strlen(lineStr) > 38){
     if (noticias[story].content[strPtr] !=0x20 || noticias[story].content[strPtr+1] !=0x20 || noticias[story].content[strPtr-1] !=0x20){
        for (k=38; k>0; k--){
                //if (lineStr[k] == '.') break;
                if (lineStr[k] == 0x20) break;
                lineStr[k]=0x20;
                strPtr--;
        }
     }
   }
   listBox1 = addatend(listBox1, newitem(lineStr));
   strcpy(lineStr,"\0");
  } while (noticias[story].content[strPtr] != '\0');

  scrollData.selectorLimit=39;    //No. of chars per item displayed
  if (listBox1 != NULL) ch = listBox(listBox1, (d_columns/2)-18, (d_rows/2)-4, &scrollData, B_BLACK, F_WHITE, B_BLACK,F_YELLOW, 9, LOCKED);
  listBox1= NULL; 
  removeList(&listBox1);
  addItems(&listBox1);
  scrollData.displayMetrics=1;
  //scrollData.selectorLimit=d_columns-12;    //No. of chars per item displayed
  deleteList(&screen1);
  create_screen(&screen1);
  copy_screen(screen1,screen2);
  dump_screen(screen1);
  //draw_screen();
  scrollData.selectorLimit=d_columns-12;    //No. of chars per item displayed
}

void display_help(){
char ch=0;
  if (screen2 != NULL) deleteList(&screen2);
  create_screen(&screen2);
  copy_screen(screen2,screen1);
  clean_bars();
  write_str(screen1,54,2,"ESC|INTRO: VOLVER", B_BLACK, F_WHITE,FALSE);
  draw_window(screen1,(d_columns/2)-25,(d_rows/2)-6,(d_columns/2) + 25,(d_rows/2)+6,B_CYAN,F_BLACK,B_BLACK,1,0,1,0);
  dump_screen(screen1);
  listBox1= NULL; 
  removeList(&listBox1);
  scrollData.displayMetrics=0;
  for (int i=0; i<HELP_LINES; i++){
    listBox1 = addatend(listBox1, newitem(help_msg[i]));
  }
  scrollData.selectorLimit=49;    //No. of chars per item displayed
  if (listBox1 != NULL) ch = listBox(listBox1, (d_columns/2)-23, (d_rows/2)-5, &scrollData, B_CYAN, F_BLACK, B_CYAN,F_WHITE, 11, LOCKED);
  listBox1= NULL; 
  if (ch==ENDSIGNAL) status=ENDSIGNAL;
  removeList(&listBox1);
  addItems(&listBox1);
/*
    do{
     ch = readch();
     if (ch == K_ESCAPE) break;
     if (ch == K_CTRL_C) {status= ENDSIGNAL; break;} 
     if (timerC(&timer1) == TRUE){
	      //Animation
              if(tick() == -1) break;
     }    
  } while (ch != K_ENTER);
*/  
  scrollData.displayMetrics=1;
  //scrollData.selectorLimit=d_columns-12;    //No. of chars per item displayed
  deleteList(&screen1);
  create_screen(&screen1);
  copy_screen(screen1,screen2);
  dump_screen(screen1);
  //draw_screen();
  scrollData.selectorLimit=d_columns-12;    //No. of chars per item displayed
}
/*-----------------------------------------*/
/* Manage keys that send a ESC sequence    */
/*-----------------------------------------*/

int special_keys(char ch) {
/* MANAGE SPECIAL KEYS */
/* 
   New implementation: Trail of chars found in keyb.c
   If K_ESCAPE is captured read a trail up to 5 characters from the console.
   This is to control the fact that some keys may change
   according to the terminal and expand the editor's possibilities.
   Eg: F2 can be either 27 79 81 or 27 91 91 82.  
   - Note : if (currentColumn > 1) cleanArea(1);	
	When horizontal scroll is active all the screen is cleaned when moving.
*/

  char    chartrail[5];
  if(ch == K_ESCAPE) {
    read_keytrail(chartrail);	//Read trail after ESC key

    //Check key trails for special keys.


    //FUNCTION KEYS : F1 - F4
    if(strcmp(chartrail, K_F2_TRAIL) == 0 ||
       strcmp(chartrail, K_F2_TRAIL2) == 0) {
      //update screen
      
    } else if(strcmp(chartrail, K_F1_TRAIL) == 0 ||
	      strcmp(chartrail, K_F1_TRAIL2) == 0) {
      display_help(); 
      // ARROW KEYS
    } else if(strcmp(chartrail, K_LEFT_TRAIL) == 0 ) {
      //Left-arrow key
       status=2; 
    } else if(strcmp(chartrail, K_RIGHT_TRAIL) == 0) {
      status=2;
    } else if(strcmp(chartrail, K_UP_TRAIL) == 0) {
      status=2;
    } else if(strcmp(chartrail, K_DOWN_TRAIL) == 0)  {
      status=2;
     } else if(strcmp(chartrail, K_PAGEDOWN_TRAIL) == 0)   {
      
     } else if(strcmp(chartrail, K_PAGEUP_TRAIL) == 0)  {
       
     } else if((strcmp(chartrail, K_HOME_TRAIL) == 0 ||
	      strcmp(chartrail, K_HOME_TRAIL2) == 0) ) {
	
     } else if((strcmp(chartrail, K_END_TRAIL) == 0 ||
	      strcmp(chartrail, K_END_TRAIL2) == 0) ) {
	
     } else if(strcmp(chartrail, K_ALT_F) == 0) {
    } else if(strcmp(chartrail, K_ALT_H) == 0) {
      display_help(); 
    } else if(strcmp(chartrail, K_ALT_I) == 0) {
    } else if(strcmp(chartrail, ESC2X) == 0) {
	return ENDSIGNAL;
    } else if(strcmp(chartrail, "\e") == 0) {
	return ENDSIGNAL;
    } else if(strcmp(chartrail, K_ALT_X) == 0) {
      return ENDSIGNAL;
    } else if(strcmp(chartrail, K_ALT_O) == 0) {
      }
    }
 return 0;
}
int main(){
  char    returnch=0;
char ch=0;
int i=0;
int keypressed = 0;
//init values
  setlocale(LC_ALL, "");
  scrollData.scrollActive=0;    //To know whether scroll is active or not.
  scrollData.scrollLimit=0;     //Last index for scroll.
  scrollData.listLength=0;      //Total no. of items in the list
  scrollData.currentListIndex=0;    //Pointer to new sublist of items when scrolling.
  scrollData.displayLimit=0;    //No. of elements to be displayed.
  scrollData.selectorLimit=d_columns-12;    //No. of chars per item displayed
  scrollData.scrollDirection=0; //To keep track of scrolling Direction.
  scrollData.selector=0;        //Y++
  scrollData.wherex=0;
  scrollData.wherey=0;
  scrollData.backColor0=0;      //0 unselected; 1 selected
  scrollData.foreColor0=0;
  scrollData.backColor1=0;
  scrollData.foreColor1=0;
  scrollData.itemIndex=0;
  scrollData.displayMetrics=1;
  //MENEAME
    download();
    creaArrayNoticias();
    addItems(&listBox1);
    //Init timer
    init_timer(&timer1,TIMER_SPEED);
    //Init Terminal
    init_term();
    draw_screen();
    resetch();
     do{    
         keypressed = kbhit(10);
          
         get_terminal_dimensions(&newsc_rows,&newsc_columns);

         if (keypressed == TRUE) 
          ch=readch();
           if (special_keys(ch) == ENDSIGNAL) status = ENDSIGNAL;
	
                
	      if (newsc_rows != d_rows || newsc_columns != d_columns)
               {
		//Refresh screen size buffers if terminal dimension changes
		   i++;
 	           draw_screen();
                   scrollData.selectorLimit=d_columns-12;    //No. of chars per item displayed
                   dump_screen(screen1);
		   status= 0;
             }  
	 if (timerC(&timer1) == TRUE){
	      //Animation
              tick();
              if (returnch== ENDSIGNAL) {status = ENDSIGNAL; break;}
	      ch=0;
           }
           if (status == 2 ){
		   returnch=list();
		   status=0;
                   if (returnch== ENDSIGNAL) {status = ENDSIGNAL; break;}
                   if ((returnch!=27) && (scrollData.itemIndex != -1)) display_story(scrollData.itemIndex);  
                   //draw_screen();
                   dump_screen(screen1);
		   ch = 0;
	   }	   
           if (ch=='i') {
               if (screen2 != NULL) deleteList(&screen2);
               create_screen(&screen2);
               copy_screen(screen2,screen1);
	       dump_screen(screen1);
               draw_window(screen1,(d_columns/2)-12,(d_rows/2)-1,(d_columns/2)+12,(d_rows/2)+1,B_YELLOW,F_BLACK,B_BLACK,1,0,1,0);
               ch=textbox(screen1, (d_columns/2)-11,(d_rows/2),3, "Ir a historia #: ", texto, B_YELLOW,F_BLACK, F_BLACK,FALSE,LOCKED);
 	       deleteList(&screen1);
               create_screen(&screen1);
               copy_screen(screen1,screen2);
               dump_screen(screen1); 
               if (ch==ENDSIGNAL) status =ENDSIGNAL; 
       	       if (strlen(texto) > 0) {
		      int num = atoi(texto)-1;
                     if (num > -1 && num < 101) display_story(num);
               }
                 ch=0;
            }
           if (ch == K_ENTER || ch == K_ENTER2) {
                  status=2;
            }
          if (ch == K_CTRL_C) status = ENDSIGNAL;
       } while (status != ENDSIGNAL);     
     //free memory
     if (screen1 != NULL) deleteList(&screen1);
     if (screen2 != NULL) deleteList(&screen2);
     //restore terminal
     close_term();
     if(listBox1 != NULL)
      removeList(&listBox1);
     //printf("\n %c\n",ch2);
     return 0;
}
