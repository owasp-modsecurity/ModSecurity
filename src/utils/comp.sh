gcc -c -std=c99 -o md5.o md5.c 
g++ -Wall -W -O2 md5.o hexify.cc md5.cpp hexify.h -o md5
