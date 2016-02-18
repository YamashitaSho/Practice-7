#include <stdio.h>
#include <string.h>
#include "header.h"

//構造体配列pの内容を表示する
void struct_print(user_t *p){
	int i = 0;
	printf("\n構造体の内容を表示します。\n");
	while(strcmp((p+i)->name,"\0") != 0){
		printf("氏名:%s\nかな:%s\nTEL :%s\n",(p+i)->name,(p+i)->ruby,(p+i)->phone);
		i++;
		if (i == DATA_NUM){
			break;
		}
	}
}