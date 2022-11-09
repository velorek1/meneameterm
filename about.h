/* 
 * Help and about information
 * Last modified: 09/11/2022 
 * @author: velorek
 *
 */
#ifndef _ABOUT_H_
#define _ABOUT_H_


#define ALINE1 "___  ___ _____ _   _  _____  ___  ___  ___ _____ " 
#define ALINE2 "|  \\/  ||  ___| \\ | ||  ___|/ _ \\ |  \\/  ||  ___|"
#define ALINE3 "| .  . || |__ |  \\| || |__ / /_\\ \\| .  . || |__  "
#define ALINE4 "| |\\/| ||  __|| . ` ||  __||  _  || |\\/| ||  __| "
#define ALINE5 "| |  | || |___| |\\  || |___| | | || |  | || |___ "
#define ALINE6 "\\_|  |_/\\____/\\_| \\_/\\____/\\_| |_/\\_|  |_/\\____/ "
                     
#define HLINE1  "   [+] M|E|N|E|A|M|E para T3rminal- by v3l0r3k   "
#define HLINE2  "================================================="
#define HLINE3  "Este programa es una [[demo]] que permite visuali"
#define HLINE4  "zas las primeras 100 historias que aparecen en la"
#define HLINE5  "portada de la pagina hispana https://meneame.net "                           
#define HLINE6  "                                                 "                                                      
#define HLINE7  "Teclas habituales:                               "                                                      
#define HLINE8  "[Ctrl+C] Sale del programa en cualquier pantalla."           
#define HLINE9  "[ESC] Regresa a la pantalla anterior o abandona. "           
#define HLINE10 "[i] Abre cuadro de texto para ir a la noticia.   "           
#define HLINE11 "[F1] Muestra la pantalla de ayuda.               "           
#define HLINE12 "[CURSOR] Permite navegar textos con scroll.      "           
#define HLINE13 "                                                 "           
#define HLINE14 "Peace!                                           "           
#define HLINE15 "Coded in [C] & [Vim] on Linux v0.1 2022          "           

                                                 
#define ABOUT_LENGTH 100
#define ABOUT_LINES 6

#define HELP_LENGTH 255
#define HELP_LINES 15



char about_msg[ABOUT_LINES][ABOUT_LENGTH] =
               {
         ALINE1,
         ALINE2,
         ALINE3,
         ALINE4,
         ALINE5,
         ALINE6
        };

char help_msg[HELP_LINES][HELP_LENGTH] =
       { HLINE1,
         HLINE2,
         HLINE3,
         HLINE4,
         HLINE5,
         HLINE6,
         HLINE7,
         HLINE8,
         HLINE9,
         HLINE10,
	 HLINE11,
         HLINE12,
         HLINE13,
         HLINE14,
         HLINE15
	 };
 
#endif
