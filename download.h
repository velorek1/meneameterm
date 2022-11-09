/*
========================================================================
- HEADER -
Modulo para descargar JSON de la API de meneame.
@author : Velorek
@version : 1.0
Last modified: 31/10/2022
========================================================================
*/

#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>


static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
int download(void);

#endif


