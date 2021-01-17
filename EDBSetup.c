#include "Head.h"

unsigned char *  EDBSetup(unsigned char* Ks_EDB, Word *KeyWord ,Pair(*EDB_Tset)[128], Word (*LargeT)[128]) {

	
	int i, j, l, tw, x;

	unsigned char e[16];
	unsigned char Ke_EDB[16];
	unsigned char* Kt_EDB = (unsigned char*)malloc((sizeof(unsigned char) * 16));
	unsigned char tmp;
	Word LargeT_EDB[128][128];//라지티
	Word V[10] = { '\0' };//V배열사이즈 
	
	memset(V, NULL, sizeof(V));
	memset(LargeT_EDB, NULL, sizeof(LargeT_EDB));
	memset(Ke_EDB, NULL, sizeof(Ke_EDB));
	memset(e, NULL, sizeof(e));

	for (i = 0; i < 16; i++) { //랜덤한 Ks_EDB생성
		tmp = rand() % 256;
		memcpy(Ks_EDB+i, &tmp, 1);
	}

	printf("\nKs= ");
	for (x = 0; x < 16; x++) {
		printf("%x ", Ks_EDB[x]);
	}

	i = 0;
	tw = 0;
	l = 0;
	
	while (strcmp(KeyWord[i].a, "") != 0) {
		printf("\n\n%s", KeyWord[i].a);
		memcpy(LargeT_EDB[i][0].a, (KeyWord+i)->a, 16);
		AES_ECB_Encrypt(KeyWord[i].a, Ks_EDB, Ke_EDB, 16);//키워드를 Ks_EDB로 암호화해서 Ke_EDB생성
		printf("\n\nKe= ");
		for (x = 0; x < 16; x++) {
			printf("%x ", Ke_EDB[x]);
		}

		printf("\n%s가 들어간 파일 목록\n", KeyWord[i].a);
		while (1) {//V배열에 키워드가 들어있는 id복사
			if (strcmp(LargeT[i][tw + 1].a, "") == 0) break;
			memcpy(V[l].a, LargeT[i][tw + 1].a, 16);
			printf("%s ", V[l].a);
			tw++;
			l++;
		}

		for (j = 0; j < tw; j++) {
			AES_ECB_Encrypt(V[j].a, Ke_EDB, e, 16); //id를 Ke_EDB로 암호화
			printf("\n");
			printf("%s를 암호화한 e%d= ", V[j].a, j);
			for (x = 0; x < 16; x++) {
				printf("%x ", e[x]);
			}
			memcpy(LargeT_EDB[i][j + 1].a, e, 16);//LargeT_EDB에 e복사
			memset(e, NULL, sizeof(e)); //e초기화
		}
		tw = 0;
		l = 0;
		i++;
		memset(V, NULL, sizeof(V));//V초기화
		memset(Ke_EDB, NULL, sizeof(Ke_EDB)); //Ke_EDB초기화
		
	}


	Kt_EDB = CreateTset(LargeT_EDB, KeyWord, EDB_Tset); //Tset생성

	return Kt_EDB;
	
}