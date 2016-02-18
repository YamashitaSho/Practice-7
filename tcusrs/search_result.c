#include <stdio.h>
#include <string.h>
#include "header.h"

//検索結果の表示-------------------------------------------------------------------
//構造体の配列のうち、一致するものがあった要素を表示する
void search_result(user_t *p, char *matching, int matchnumber){
	int i = 0;
	if (matchnumber > 0){
		printf("\nファイルの検索結果を表示します。\n\n");
		while(strcmp((p+i)->name,"\0") != 0){
			if ( (matching[i]) != 0 ){		//True
				printf("氏名:%s\nかな:%s\nTEL :%s\n",(p+i)->name,(p+i)->ruby,(p+i)->phone);
			}
			i++;
			if (i > DATA_NUM){
				break;
			}
		}
		printf("\n");
	} else {
		printf("\nワードは見つかりませんでした。\n\n");
		return;
	}
}
