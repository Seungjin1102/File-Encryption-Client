#include "Head.h"


unsigned char* CreateTset(Word (*LargeT)[128], Word *KeyWord, Pair (*Tset)[128]) {

	int i, j, k, l, m, n,z ;
	int tw;
	Word V[10] = { '\0' };//V�迭������ 
	unsigned char Stag[16] = { '\0' };
	unsigned char EncryptStag[16] = { '\0' };//Stag�� AES
	unsigned char hashStag[32] = { '\0' };//Stag�� �ؽ�
	unsigned char SecretKey[16] = { '\0' };
	char* b = malloc(sizeof(char));
	unsigned char x[16]= { '\0' };
	unsigned char res;
	unsigned char L[10];
	unsigned char K[17]={ '\0' };
	unsigned char tmpValue[32]= { '\0' };
	unsigned char tmpXor[17]= { '\0' };
	unsigned char* Beta = malloc(sizeof(unsigned char));

	
	int(*Free)[128];
	Free = (int(*)[128])malloc(128 * 128 * sizeof(int));
	
	struct Flags f;

	SHA256_CTX ctx;

	memset(Free, NULL, sizeof(Free));
	memset(tmpValue, NULL, sizeof(tmpValue));

	for (i = 0; i < 128; i++) {//Free����
		for (j = 0; j < 128; j++) {
			Free[i][j] = j + 1;
		}
	}
	



	for (i = 0; i < 16; i++) { //������ SecretKey����
		SecretKey[i] = rand() % 256;
	}
	
	k = 0;

	while (strcmp(KeyWord[k].a, "") != 0) {

		AES_ECB_Encrypt(KeyWord[k].a, SecretKey, Stag, 16);//Stag����
		
		printf("\n\n%s�� Stag= ", KeyWord[k].a);
		for (z = 0; z < 16; z++) {
			printf("%x ", Stag[z]);
		}
		tw = 0;//V�迭������ 
		l = 0;

		while (1) {
			if (strcmp(LargeT[k][tw+1].a, "") == 0) break;//V�� id�� �ֱ�
			memcpy(V[l].a, LargeT[k][tw+1].a, 16);
			tw++;
			l++;
		}
		
		for (i = 0; i < tw; i++) {//Tset�� ���ֱ�                 
			
			AES_ECB_Encrypt(Stag, _itoa(i, x, 16), EncryptStag, 16);//stag�� AES
			
			SHA256_Init(&ctx);//�ؽ��Լ�
			SHA256_Update(&ctx, EncryptStag, 16);
			SHA256_Final(hashStag, &ctx);
			
			strncpy(b, hashStag, 1);//b����
			f.a = *b;//���� b���� �����κ�ȯ
			if (f.a < 0) f.a = -(f.a) - 1;
			
			memcpy(L, hashStag + 1, 10); //L����
			memcpy(K, hashStag + 11, 17); //K����
			
			j = rand() % 128;//������ j����
			Free[f.a][j] = 0;//j��° �ε����� 0����
			if(tw-i == 1) strcpy(Beta, "0");//��Ÿ�� ����
			else strcpy(Beta, "1");
			memcpy((Tset[f.a][j].label), L, 10);//L�� Tset.label�� ����
			memcpy(tmpValue, Beta, 1); //Beta��id ����
			memcpy(tmpValue + 1, V[i].a, 16);
			
			for (m = 0; m < 17; m++) {//�����Ѱ��� K�� xor�ؼ�tmpXor����
				res = tmpValue[m] ^ K[m];
				memcpy((tmpXor + m), &res, 1);
			}
			
			memcpy(Tset[f.a][j].value, tmpXor, 17);//Tset.value�� ������
			memset(tmpXor, NULL, sizeof(tmpXor));//tmpXor�ʱ�ȭ
			memset(tmpValue, NULL, sizeof(tmpValue));//tmpValue �ʱ�ȭ
			memset(K, NULL, sizeof(K));//K�ʱ�ȭ
		}
		
		memset(V, NULL, sizeof(V));//V�ʱ�ȭ
		k++;
	
	}
	free(Free);
	return SecretKey;
}



