/* 
 * MODULE to download a JSON object from Meneame's API and save it locally into 
 * a file. 
 * - Dependencies: Libcurl
 * sudo apt install libcurl4-openssl-dev
 * Last modified: 9/7/2022
 * @author: velorek
*/
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "download.h"
#include "fileop.h"
#include "rterm.h"
#include "global.h"

CURL *curl;
char buffer[CURL_ERROR_SIZE];


static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}


int download(void) {
  outputcolor(F_BLACK,B_BLACK);
  FILE *pagefile=NULL; 
  /* open the file */
  //Make a backup of previous page for backup
  if (file_exists(FICHERO1) && getfileSize(FICHERO1)>0){
      copyFile (FICHERO1, FICHERO2);
  }
  openFile(&pagefile, FICHERO1, "wb");

  if ((curl = curl_easy_init()) != NULL) {
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_URL, "https://meneame.net/api/list.php?status=published");
        curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L); // disable messages
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, buffer);
        curl_easy_perform(curl);
        if (curl_easy_perform(curl) != CURLE_OK) {
            fprintf(stderr, "%s\n", buffer);
            return EXIT_FAILURE;
        }
 
    closeFile(pagefile);
    curl_easy_cleanup(curl);
   }
    
    outputcolor(B_BLACK,F_YELLOW);
    printf("\r[+] Cargando....\n");

    return 0;
}

