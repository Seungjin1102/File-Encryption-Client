#include "Head.h"

unsigned char* TsetGetTag(unsigned char* kt, unsigned char* w) {

	
	unsigned char tmpKeyWord[16] = { '\0' };
	unsigned char Stag[16] = { '\0' };
	
	strncpy(tmpKeyWord, w, 16);
	AES_ECB_Encrypt(tmpKeyWord, kt, Stag, 16);

	return Stag;
	






}