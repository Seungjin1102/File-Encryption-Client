#define _WINSOK_DEPRECATED_NO_WARNINGS
#include "Head.h"


int main() {

	int i, j, k;
	int count = 0;
	int tw = 0;

	Word KeyWord[10]; //전체 키워드 배열
	Word FileKeyWord[4][10];//파일별 키워드 배열
	
	Word LargeT[128][128];//라지티
	Pair EDB_Tset[128][128];//티셋
	Word V[4]; //서버에서 오는 파일이름 저장
	Word D[4]; //복호화된 파일이름
	
	unsigned char *Stagptr;
	unsigned char Stag[16] = { '\0' };
	unsigned char Ke[16] = { '\0' };
	unsigned char* Kt_EDB = (unsigned char*)malloc(sizeof(unsigned char) * 16); //EDBSetup에서 키
	unsigned char* Ks_EDB = (unsigned char*)malloc(sizeof(unsigned char) * 16); //EDBSetup에서 키

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	
	char ip[16];
	int port = 0;
	int strLen;
	unsigned char test[10] = { '\0' };//y or n
	unsigned char testKeyWord[16] = { '\0' };//입력받을 키워드
	Index Index_Num[10];//EDB_Tset의 데이터가 있는 인덱스값
	int size = 0;

	memset(Index_Num, NULL, sizeof(Index_Num));

	memset(D, NULL, sizeof(D));
	memset(KeyWord, NULL, sizeof(KeyWord));
	memset(FileKeyWord, NULL, sizeof(FileKeyWord));
	memset(LargeT, NULL, sizeof(LargeT));
	memset(V, NULL, sizeof(V));
	memset(EDB_Tset, NULL, sizeof(EDB_Tset));
	
	
	FileOpen("a.txt", KeyWord, FileKeyWord, 0, (sizeof(FileKeyWord[0]) / sizeof(FileKeyWord[0][0])));//a파일 오픈
	FileOpen("b.txt", KeyWord, FileKeyWord, 1, (sizeof(FileKeyWord[0]) / sizeof(FileKeyWord[0][0])));//b파일 오픈
	FileOpen("c.txt", KeyWord, FileKeyWord, 2, (sizeof(FileKeyWord[0]) / sizeof(FileKeyWord[0][0])));//c파일 오픈
	FileOpen("d.txt", KeyWord, FileKeyWord, 3, (sizeof(FileKeyWord[0]) / sizeof(FileKeyWord[0][0])));//d파일 오픈
	
	printf("파일 키워드 목록\n");
	for (i = 0; i < 4; i++) {
		printf("id%d", i);
		for (j = 0; j < 10; j++) {
			printf(" %s", FileKeyWord[i][j].a);
		}
		printf("\n");
	}
	CreateLargeT(KeyWord, FileKeyWord, LargeT);//라지T생성
	
	srand(time(NULL));
	Kt_EDB = EDBSetup(Ks_EDB, KeyWord, EDB_Tset, LargeT);//Kt_EDB생성
	printf("\nKt_EDB= ");
	for (i = 0; i < 16; i++) {
		printf("%x ", Kt_EDB[i]);
	}
	//소켓통신시작
	memset(ip, NULL, sizeof(ip));
	printf("\n\nip를 입력하세요: ");
	scanf("%s", ip);
	printf("포트번호를 입력하세요: ");
	scanf("%d", &port);
	

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //소켓 라이브러리를 초기화하고 있다
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0); //소켓을 생성하고

	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);//ip주소
	servAddr.sin_port = htons(port);//포트번호

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //생성된 소켓을 바탕으로 서버에 연결요청을 하고 있다
		ErrorHandling("connect() error!");
	

	printf("\n\nEDB_Tset을 서버로 보내시겠습니까?(y/n)");
	scanf("%s", test);
	if (strncmp(test, "y", 1) == 0) {

		size = SearchIndex(EDB_Tset, Index_Num);//EDB_Tset에 들어가있는 요소 size
		send(hSocket, &size, sizeof(size), 0); 
		send(hSocket, Index_Num, sizeof(Index_Num), 0); //EDB_Tset 서버로 전송
		for (i = 0; i < size; i++) {
			send(hSocket, EDB_Tset[Index_Num[i].i][Index_Num[i].j].label, 27, 0);
		}
	}
	

	while (1) {

		printf("\n\n검색할 파일의 키워드를 입력하시오: ");
		scanf("%s", testKeyWord);

		Stagptr = TsetGetTag(Kt_EDB, testKeyWord);//키워드에 따른 Stag 얻기
		for (i = 0; i < 16; i++) {
			memmove(Stag + i, Stagptr + i, 1);
		}


		printf("\n%s의 Stag = ", testKeyWord);
		for (i = 0; i < 16; i++) {
			printf("%x ", Stag[i]);
		}
		printf("\n\nStag값을 서버로 보내시겠습니까?(y/n)");
			scanf("%s", test);
		if (strncmp(test, "y", 1) == 0) {
			send(hSocket, Stag, sizeof(Stag), 0); //Stag 서버로 전송
		}

		printf("\n암호화된 파일id를 받으시겠습니까?(y/n)");
		scanf("%s", test);
		if (strncmp(test, "y", 1) == 0) {
			strLen = recv(hSocket, V, sizeof(V), 0);//서버로부터 암호화된 파일id받기
			if (strLen == -1)
				ErrorHandling("read() error");
		}

		printf("\n암호화된 파일id\n");
		for (i = 0; i < 4; i++) {
			if (strcmp(V[i].a, "") != 0) {
				printf("e%d= ", i);
				for (j = 0; j < 16; j++) {
					printf("%x ", *((V[i].a) + j));
				}
				printf("\n");
			}

		}

		while (strcmp(V[tw].a, "") != 0) tw++;

		AES_ECB_Encrypt(testKeyWord, Ks_EDB, Ke, 16); //키워드와 Ks_EDB로 암호화된 Ke(암호화된id)생성

		printf("\nKe= ");
		for (j = 0; j < 16; j++) {
			printf("%x ", Ke[j]);
		}
		printf("\n");

		for (i = 0; i < tw; i++) {
			AES_ECB_Decrypt(V[i].a, Ke, D[i].a, 16); //Ke 복호화
		}

		printf("\n복호화된 파일id\n");
		for (i = 0; i < 4; i++) {
			if (strncmp(D[i].a, "", 16) != 0) {
				printf("%s\n", D[i].a);
			}
		}
		memset(testKeyWord, NULL, sizeof(testKeyWord));
		memset(Ke, NULL, sizeof(Ke));
		memset(D, NULL, sizeof(D));
		memset(V, NULL, sizeof(V));
		tw = 0;
		printf("\n다른키워드를 검색하시겠습니까?(y/n)");
		scanf("%s", test);
		if (strcmp(test, "y") != 0) break;

	}
	closesocket(hSocket); //소켓 라이브러리 해제
	WSACleanup();
	return 0;
}



void ErrorHandling(char* message) //에러

{

	fputs(message, stderr);

	fputc('\n', stderr);

	exit(1);

}