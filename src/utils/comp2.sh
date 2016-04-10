gcc -c -std=c99 -o md5.o md5.c 
g++ -Wall -W -O2 md5.o hexify.h hexify.cc  md52.cpp crypto_helpers.cc crypto_helpers.h -o md52
