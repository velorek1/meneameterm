#!/bin/bash
# file: build.sh
echo -e "\e[33mMeneame for Terminals - Coded By v3l0r3k\e[0m"
echo
read -p "[+] Install libcurl libraries? (sudo required) [Y/N]: " -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
 sudo apt-get install libcurl4 && sudo apt-get install libcurl4-openssl-dev
fi
echo
echo -e "Run as \e[33m./meneame\e[0m" 
echo Enjoy it!
gcc -c rterm.c
gcc -c -funsigned-char scbuf.c
gcc -c global.h
gcc -c about.h
gcc -c ui.c
gcc -c tm.c
gcc -c -funsigned-char main.c
gcc -c download.c
gcc -c -funsigned-char parse.c
gcc -c -funsigned-char listbox.c
gcc -c keyb.c
gcc -c fileop.c

gcc rterm.o tm.o fileop.o ui.o scbuf.o listbox.o keyb.o parse.o download.o main.o -o meneame -funsigned-char -lcurl
