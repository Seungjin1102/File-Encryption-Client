#include "Head.h"

unsigned char *  EDBSetup(unsigned char* Ks_EDB, Word *KeyWord ,Pair(*EDB_Tset)[128], Word (*LargeT)[128]) {

	
	int i, j, l, tw, x;

	unsigned char e[16];
	unsigned char Ke_EDB[16];
	unsigned char* Kt_EDB = (unsigned char*)malloc((sizeof(unsigned char) * 16));
	unsigned char tmp;
	Word LargeT_EDB[128][128];//����Ƽ
	Word V[10] = { '\0' };//V�迭������ 
	
	memset(V, NULL, sizeof(V));
	memset(LargeT_EDB, NULL, sizeof(LargeT_EDB));
	memset(Ke_EDB, NULL, sizeof(Ke_EDB));
	memset(e, NULL, sizeof(e));

	for (i = 0; i < 16; i++) { //������ Ks_EDB����
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
		AES_ECB_Encrypt(KeyWord[i].a, Ks_EDB, Ke_EDB, 16);//Ű���带 Ks_EDB�� ��ȣȭ�ؼ� Ke_EDB����
		printf("\n\nKe= ");
		for (x = 0; x < 16; x++) {
			printf("%x ", Ke_EDB[x]);
		}

		printf("\n%s�� �� ���� ���\n", KeyWord[i].a);
		while (1) {//V�迭�� Ű���尡 ����ִ� id����
			if (strcmp(LargeT[i][tw + 1].a, "") == 0) break;
			memcpy(V[l].a, LargeT[i][tw + 1].a, 16);
			printf("%s ", V[l].a);
			tw++;
			l++;
		}

		for (j = 0; j < tw; j++) {
			AES_ECB_Encrypt(V[j].a, Ke_EDB, e, 16); //id�� Ke_EDB�� ��ȣȭ
			printf("\n");
			printf("%s�� ��ȣȭ�� e%d= ", V[j].a, j);
			for (x = 0; x < 16; x++) {
				printf("%x ", e[x]);
			}
			memcpy(LargeT_EDB[i][j + 1].a, e, 16);//LargeT_EDB�� e����
			memset(e, NULL, sizeof(e)); //e�ʱ�ȭ
		}
		tw = 0;
		l = 0;
		i++;
		memset(V, NULL, sizeof(V));//V�ʱ�ȭ
		memset(Ke_EDB, NULL, sizeof(Ke_EDB)); //Ke_EDB�ʱ�ȭ
		
	}


	Kt_EDB = CreateTset(LargeT_EDB, KeyWord, EDB_Tset); //Tset����

	return Kt_EDB;
	
}