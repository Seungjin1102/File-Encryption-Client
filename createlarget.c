#include "Head.h"

void CreateLargeT(Word *KeyWord, Word (*FileKeyWord)[10], Word(*LargeT)[128]) {

	int i, j, k, x ,y;
	int count = 0;
	int flag = 0;

	for (i = 0; i < 4; i++) {//KeyWord배열에 키워드 중복되지 않게 복사
		for (j = 0; j < 10; j++) {
			k = 0;
			while (k <= count) {
				flag = strcmp(FileKeyWord[i][j].a, KeyWord[k].a);
				if (flag == 0) break;

				else {
					if (strcmp(KeyWord[k].a, "") == 0) {
						strcpy(KeyWord[k].a, FileKeyWord[i][j].a);
						count++;
						break;
					}
					else k++;
				}

			}
		}
	}
	k = 0;
	j = 0;

	
	for (i = 0; i < 10; i++) {//LargeT생성
		strcpy(LargeT[i][0].a, KeyWord[i].a);
	}
	i = 0;
	count = 0;

	while (i < 5) {//LargeT 생성
		count = 1;
		for (x = 0; x < 4; x++) {
			for (y = 0; y < 10; y++) {
				if (strcmp(LargeT[i][0].a, FileKeyWord[x][y].a) == 0) {
					switch (x) {
					case 0:
						strcpy(LargeT[i][count].a, "id0");
						count++;
						break;
					case 1:
						strcpy(LargeT[i][count].a, "id1");
						count++;
						break;
					case 2:
						strcpy(LargeT[i][count].a, "id2");
						count++;
						break;
					case 3:
						strcpy(LargeT[i][count].a, "id3");
						count++;
						break;
					default :
						break;
					}
				}
			}
		}
		i++;
	}


	
}