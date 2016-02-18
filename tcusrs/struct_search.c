#include <stdio.h>
#include <string.h>
#include "header.h"

/* struct_search(user_t *p)
ワードを入力させ、構造体pの中から検索する。

動作:
keywordにワードをもらう(input_a_line)
name,ruby,phoneの順で0以外が帰ってくるまで検索し、0以外が帰ってきた場合にカウントする。
最後に検索結果を表示する(search_result)
*/
int struct_search(user_t *p){
	char keyword[256] = DEFAULT_WORD;
	char matching[DATA_NUM+1];						//発見データ
	int i = 0;										//構造体配列番号と共に動くループ変数
	int matchnumber = 0;							//発見回数
	
	memset(matching,MATCHING_FALSE,DATA_NUM+1);
	memset(keyword,'\0',256);
	
	input_a_line(keyword , MODE_SEARCH);			//ワードをもらってくる
	if ( keyword[0] == '\n' ){
		return NO_WORD_ERROR;
	}
	while(strcmp((p+i)->name,"\0") != 0){
		
		if ( !( matching[i] = check((p+i)->name, keyword)) ) {		//nameとkeywordを比べて、マッチしない
			if ( !(matching[i] = check((p+i)->ruby, keyword)) ) {	//rubyとkeywordを比べて、マッチしない
				matching[i] = check((p+i)->phone, keyword);				//phoneとkeywordを比べた結果を入れる
			}											//name,rubyと一致した場合はこの部分はMATCHING_TRUEが入っているはず
		}
		if (matching[i]){						//TRUE
			matchnumber ++;
		}
		i++;
		if (i > DATA_NUM - 1){						// DATA_NUM-1 番目までしか存在しない
			break;
		}
	}

	search_result( p , matching , matchnumber);
	return 0;
}
