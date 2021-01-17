#include "Head.h"


unsigned char* CreateTset(Word (*LargeT)[128], Word *KeyWord, Pair (*Tset)[128]) {

	int i, j, k, l, m, n,z ;
	int tw;
	Word V[10] = { '\0' };//V배열사이즈 
	unsigned char Stag[16] = { '\0' };
	unsigned char EncryptStag[16] = { '\0' };//Stag를 AES
	unsigned char hashStag[32] = { '\0' };//Stag를 해쉬
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

	for (i = 0; i < 128; i++) {//Free생성
		for (j = 0; j < 128; j++) {
			Free[i][j] = j + 1;
		}
	}
	



	for (i = 0; i < 16; i++) { //랜덤한 SecretKey생성
		SecretKey[i] = rand() % 256;
	}
	
	k = 0;

	while (strcmp(KeyWord[k].a, "") != 0) {

		AES_ECB_Encrypt(KeyWord[k].a, SecretKey, Stag, 16);//Stag생성
		
		printf("\n\n%s의 Stag= ", KeyWord[k].a);
		for (z = 0; z < 16; z++) {
			printf("%x ", Stag[z]);
		}
		tw = 0;//V배열사이즈 
		l = 0;

		while (1) {
			if (strcmp(LargeT[k][tw+1].a, "") == 0) break;//V에 id값 넣기
			memcpy(V[l].a, LargeT[k][tw+1].a, 16);
			tw++;
			l++;
		}
		
		for (i = 0; i < tw; i++) {//Tset에 값넣기                 
			
			AES_ECB_Encrypt(Stag, _itoa(i, x, 16), EncryptStag, 16);//stag를 AES
			
			SHA256_Init(&ctx);//해쉬함수
			SHA256_Update(&ctx, EncryptStag, 16);
			SHA256_Final(hashStag, &ctx);
			
			strncpy(b, hashStag, 1);//b추출
			f.a = *b;//문자 b값을 정수로변환
			if (f.a < 0) f.a = -(f.a) - 1;
			
			memcpy(L, hashStag + 1, 10); //L생성
			memcpy(K, hashStag + 11, 17); //K생성
			
			j = rand() % 128;//랜덤한 j선택
			Free[f.a][j] = 0;//j번째 인덱스를 0으로
			if(tw-i == 1) strcpy(Beta, "0");//베타값 설정
			else strcpy(Beta, "1");
			memcpy((Tset[f.a][j].label), L, 10);//L을 Tset.label에 저장
			memcpy(tmpValue, Beta, 1); //Beta와id 연접
			memcpy(tmpValue + 1, V[i].a, 16);
			
			for (m = 0; m < 17; m++) {//연접한것을 K와 xor해서tmpXor저장
				res = tmpValue[m] ^ K[m];
				memcpy((tmpXor + m), &res, 1);
			}
			
			memcpy(Tset[f.a][j].value, tmpXor, 17);//Tset.value에 값설정
			memset(tmpXor, NULL, sizeof(tmpXor));//tmpXor초기화
			memset(tmpValue, NULL, sizeof(tmpValue));//tmpValue 초기화
			memset(K, NULL, sizeof(K));//K초기화
		}
		
		memset(V, NULL, sizeof(V));//V초기화
		k++;
	
	}
	free(Free);
	return SecretKey;
}



