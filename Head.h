#define _CRT_SECURE_NO_WARNINGS

#define KEYWORD_SIZE 5
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "Windows.h"
#include "wincrypt.h"
#include "openssl/sha.h"


struct Flags {
	int a : 8; //createTsetÀÇ b°ª
};

typedef struct word {
	unsigned char a[16];
}Word;

typedef struct pair {
	unsigned char label[10];
	unsigned char value[17];
}Pair;

typedef struct index {
	int i;
	int j;
}Index;


void FileOpen(char *, Word*, Word* [10], int , int  );
void CreateLargeT(Word*, Word* [10], Word *[128]);
unsigned char* CreateTset(Word*[128], Word*, Pair *[128]);
unsigned char* TsetGetTag(unsigned char* kt, unsigned char* w);
void ErrorHandling(char*);
unsigned char * EDBSetup(unsigned char* Ks_EDB, Word* KeyWord, Pair(*EDB_Tset)[128], Word(*)[128]);
int SearchIndex(Pair(*EDB_Tset)[128], Index* num);