#include "Head.h"

void FileOpen(char* txt, Word* KeyWord, Word(*FileKeyWord)[10], int FileNum, int size) {

	FILE* fp = NULL;
	char String[100] = { "\0" };
	char* ptr;//�ؽ�Ʈ ���� ������
	char* tmp[10] = { "\0" };
	int i = 0;
	int j = 0;



	fp = fopen(txt, "r");//�ؽ�Ʈ ���� ����
	while (1) {//�ӽ� �迭 String�� Ű���� ����
		String[i] = fgetc(fp);
		if (String[i] == EOF) {
			String[i] = '\0';
			break;
		}
		i++;
	}

	ptr = strtok(String, " ");//Ű���� �ڸ���
	i = 0;
	while (ptr != NULL) {
		tmp[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");

	}
	fclose(fp);

	switch (FileNum) {//FileKeyWord�迭�� ���Ϻ��� Ű���� ����
	case 0:
		for (i = 0; i < size; i++) {
			if (tmp[i] == NULL) break;
			strcpy(FileKeyWord[FileNum][i].a, tmp[i]);
		}
		break;

	case 1:
		for (i = 0; i < size; i++) {
			if (tmp[i] == NULL) break;
			strcpy(FileKeyWord[FileNum][i].a, tmp[i]);
		}
		break;

	case 2:
		for (i = 0; i < size; i++) {
			if (tmp[i] == NULL) break;
			strcpy(FileKeyWord[FileNum][i].a, tmp[i]);
		}
		break;
	case 3:
		for (i = 0; i < size; i++) {
			if (tmp[i] == NULL) break;
			strcpy(FileKeyWord[FileNum][i].a, tmp[i]);
		}
		break;
	default:
		break;
	}


}