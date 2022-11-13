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
#define HLINE3  "Este programa es una [[demo]] que permite        "
#define HLINE4  "visualizar las primeras 100 historias que        "
#define HLINE5  "aparecen eb la portada de la pagina hispana      "                           
#define HLINE6  "https://meneame.net                              "                                                      
#define HLINE7  "                                                 "                                                      
#define HLINE8  "Teclas habituales:                               "                                                      
#define HLINE9  "[Ctrl+C] Sale del programa en cualquier pantalla."           
#define HLINE10  "[ESC] Regresa a la pantalla anterior o abandona. "           
#define HLINE11 "[i] Abre cuadro de texto para ir a la noticia.   "           
#define HLINE12 "[F1] Muestra la pantalla de ayuda.               "           
#define HLINE13 "[CURSOR] Permite navegar textos con scroll.      "           
#define HLINE14 "                                                 "           
#define HLINE15 "Peace!                                           "           
#define HLINE16 "Coded in [C] & [Vim] on Linux v0.1 2022          "           

                                                 
#define ABOUT_LENGTH 100
#define ABOUT_LINES 6

#define HELP_LENGTH 255
#define HELP_LINES 16



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
         HLINE15,
         HLINE16
	 };
 
#endif
