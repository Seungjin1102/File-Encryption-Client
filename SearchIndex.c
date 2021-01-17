#include "Head.h"

int SearchIndex(Pair(*EDB_Tset)[128], Index* Index_Num) {

	int i, j;
	int size = 0;
	int k = 0;

	for (i = 0; i < 128; i++) {
		for (j = 0; j < 128; j++) {
			if (strcmp(EDB_Tset[i][j].label, "") != 0) {
				printf("\n\nEDB_Tset[%d][%d].label= ", i, j);
				for (k = 0; k < 27; k++) {
					if (k < 10) printf("%x ", *((EDB_Tset[i][j].label) + k));
					else if (k == 10) printf("\nvalue= %x ", *((EDB_Tset[i][j].label) + k));
					else printf("%x ", *((EDB_Tset[i][j].label) + k));
				}
				Index_Num[size].i = i;
				Index_Num[size].j = j;
				size++;
			}
		}
	}


		return size;
}