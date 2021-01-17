#include "Head.h"

void FileOpen(char* txt, Word* KeyWord, Word(*FileKeyWord)[10], int FileNum, int size) {

	FILE* fp = NULL;
	char String[100] = { "\0" };
	char* ptr;//텍스트 파일 포인터
	char* tmp[10] = { "\0" };
	int i = 0;
	int j = 0;



	fp = fopen(txt, "r");//텍스트 파일 열기
	while (1) {//임시 배열 String에 키워드 저장
		String[i] = fgetc(fp);
		if (String[i] == EOF) {
			String[i] = '\0';
			break;
		}
		i++;
	}

	ptr = strtok(String, " ");//키워드 자르기
	i = 0;
	while (ptr != NULL) {
		tmp[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");

	}
	fclose(fp);

	switch (FileNum) {//FileKeyWord배열에 파일별로 키워드 저장
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