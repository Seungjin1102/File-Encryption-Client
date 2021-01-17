#define _WINSOK_DEPRECATED_NO_WARNINGS
#include "Head.h"


int main() {

	int i, j, k;
	int count = 0;
	int tw = 0;

	Word KeyWord[10]; //��ü Ű���� �迭
	Word FileKeyWord[4][10];//���Ϻ� Ű���� �迭
	
	Word LargeT[128][128];//����Ƽ
	Pair EDB_Tset[128][128];//Ƽ��
	Word V[4]; //�������� ���� �����̸� ����
	Word D[4]; //��ȣȭ�� �����̸�
	
	unsigned char *Stagptr;
	unsigned char Stag[16] = { '\0' };
	unsigned char Ke[16] = { '\0' };
	unsigned char* Kt_EDB = (unsigned char*)malloc(sizeof(unsigned char) * 16); //EDBSetup���� Ű
	unsigned char* Ks_EDB = (unsigned char*)malloc(sizeof(unsigned char) * 16); //EDBSetup���� Ű

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	
	char ip[16];
	int port = 0;
	int strLen;
	unsigned char test[10] = { '\0' };//y or n
	unsigned char testKeyWord[16] = { '\0' };//�Է¹��� Ű����
	Index Index_Num[10];//EDB_Tset�� �����Ͱ� �ִ� �ε�����
	int size = 0;

	memset(Index_Num, NULL, sizeof(Index_Num));

	memset(D, NULL, sizeof(D));
	memset(KeyWord, NULL, sizeof(KeyWord));
	memset(FileKeyWord, NULL, sizeof(FileKeyWord));
	memset(LargeT, NULL, sizeof(LargeT));
	memset(V, NULL, sizeof(V));
	memset(EDB_Tset, NULL, sizeof(EDB_Tset));
	
	
	FileOpen("a.txt", KeyWord, FileKeyWord, 0, (sizeof(FileKeyWord[0]) / sizeof(FileKeyWord[0][0])));//a���� ����
	FileOpen("b.txt", KeyWord, FileKeyWord, 1, (sizeof(FileKeyWord[0]) / sizeof(FileKeyWord[0][0])));//b���� ����
	FileOpen("c.txt", KeyWord, FileKeyWord, 2, (sizeof(FileKeyWord[0]) / sizeof(FileKeyWord[0][0])));//c���� ����
	FileOpen("d.txt", KeyWord, FileKeyWord, 3, (sizeof(FileKeyWord[0]) / sizeof(FileKeyWord[0][0])));//d���� ����
	
	printf("���� Ű���� ���\n");
	for (i = 0; i < 4; i++) {
		printf("id%d", i);
		for (j = 0; j < 10; j++) {
			printf(" %s", FileKeyWord[i][j].a);
		}
		printf("\n");
	}
	CreateLargeT(KeyWord, FileKeyWord, LargeT);//����T����
	
	srand(time(NULL));
	Kt_EDB = EDBSetup(Ks_EDB, KeyWord, EDB_Tset, LargeT);//Kt_EDB����
	printf("\nKt_EDB= ");
	for (i = 0; i < 16; i++) {
		printf("%x ", Kt_EDB[i]);
	}
	//������Ž���
	memset(ip, NULL, sizeof(ip));
	printf("\n\nip�� �Է��ϼ���: ");
	scanf("%s", ip);
	printf("��Ʈ��ȣ�� �Է��ϼ���: ");
	scanf("%d", &port);
	

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //���� ���̺귯���� �ʱ�ȭ�ϰ� �ִ�
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0); //������ �����ϰ�

	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);//ip�ּ�
	servAddr.sin_port = htons(port);//��Ʈ��ȣ

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) //������ ������ �������� ������ �����û�� �ϰ� �ִ�
		ErrorHandling("connect() error!");
	

	printf("\n\nEDB_Tset�� ������ �����ðڽ��ϱ�?(y/n)");
	scanf("%s", test);
	if (strncmp(test, "y", 1) == 0) {

		size = SearchIndex(EDB_Tset, Index_Num);//EDB_Tset�� ���ִ� ��� size
		send(hSocket, &size, sizeof(size), 0); 
		send(hSocket, Index_Num, sizeof(Index_Num), 0); //EDB_Tset ������ ����
		for (i = 0; i < size; i++) {
			send(hSocket, EDB_Tset[Index_Num[i].i][Index_Num[i].j].label, 27, 0);
		}
	}
	

	while (1) {

		printf("\n\n�˻��� ������ Ű���带 �Է��Ͻÿ�: ");
		scanf("%s", testKeyWord);

		Stagptr = TsetGetTag(Kt_EDB, testKeyWord);//Ű���忡 ���� Stag ���
		for (i = 0; i < 16; i++) {
			memmove(Stag + i, Stagptr + i, 1);
		}


		printf("\n%s�� Stag = ", testKeyWord);
		for (i = 0; i < 16; i++) {
			printf("%x ", Stag[i]);
		}
		printf("\n\nStag���� ������ �����ðڽ��ϱ�?(y/n)");
			scanf("%s", test);
		if (strncmp(test, "y", 1) == 0) {
			send(hSocket, Stag, sizeof(Stag), 0); //Stag ������ ����
		}

		printf("\n��ȣȭ�� ����id�� �����ðڽ��ϱ�?(y/n)");
		scanf("%s", test);
		if (strncmp(test, "y", 1) == 0) {
			strLen = recv(hSocket, V, sizeof(V), 0);//�����κ��� ��ȣȭ�� ����id�ޱ�
			if (strLen == -1)
				ErrorHandling("read() error");
		}

		printf("\n��ȣȭ�� ����id\n");
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

		AES_ECB_Encrypt(testKeyWord, Ks_EDB, Ke, 16); //Ű����� Ks_EDB�� ��ȣȭ�� Ke(��ȣȭ��id)����

		printf("\nKe= ");
		for (j = 0; j < 16; j++) {
			printf("%x ", Ke[j]);
		}
		printf("\n");

		for (i = 0; i < tw; i++) {
			AES_ECB_Decrypt(V[i].a, Ke, D[i].a, 16); //Ke ��ȣȭ
		}

		printf("\n��ȣȭ�� ����id\n");
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
		printf("\n�ٸ�Ű���带 �˻��Ͻðڽ��ϱ�?(y/n)");
		scanf("%s", test);
		if (strcmp(test, "y") != 0) break;

	}
	closesocket(hSocket); //���� ���̺귯�� ����
	WSACleanup();
	return 0;
}



void ErrorHandling(char* message) //����

{

	fputs(message, stderr);

	fputc('\n', stderr);

	exit(1);

}