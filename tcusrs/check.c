#include <stdio.h>
#include <string.h>
#include "header.h"

/*int check(char *data, char *keyword)
文字列dataの中に文字列keywordと一致するものがあるかどうかを返す。
あればTRUE、なければFALSEを返す。

動作:
strlen(data)回、先頭をずらしながらkeywordと完全一致するかどうかを探す。
完全一致かどうかはpcheck()を利用する。
一致した場合にはすぐTRUEを返し、規定回数終わっても見つからなければFALSEを返す。
*/

/* int pcheck(char *data, char *keyword)
文字列dataと文字列keywordが一致するかどうかを返す。
一致すればTRUE、一致しなければFALSEを返す。

動作:
*pdにdataのポインタを、*pkにkeywordのポインタを代入している。

keywordの先頭が'\0'、すなわちkeywordが空文字列である場合はTRUEを返す。
keywordの先頭が'*'ならば、dataの文字列にkeywordの残り文字列が存在するかどうかを返す。(再帰呼び出し:check())
dataの先頭が'\0'ならば、keywordが空でないので文字列の終わりと判断してFALSEを返す。

dataの先頭とkeywordの先頭が一致した場合、それぞれの文字列の残りでもう一度処理を行う。
->dataが先になくなればFALSE、keywordが先になくなればTRUEになる。
dataの先頭とkeywordの先頭が一致しなかった場合FALSEとなる。

*/
int check(char *data, char *keyword){
	int i = 0;
	char *pd = data;
	char *pk = keyword;
	
	while ( i <= strlen(data) ){
		if ( pcheck( (pd+i) , pk ) ) {
			return MATCHING_TRUE;
		}
		i++;
	}
	return MATCHING_FALSE;
}

int pcheck(char *data, char *keyword){
	char *pd = data;
	char *pk = keyword;
	
	while(1){
		if (*pk == 0) {						//キーが空
			return MATCHING_TRUE;
		} else if ( *pk == '*' ){			//キーの先頭がアスタリスク
			return check( pd, (pk+1) );			//キーのみ1バイト進めて比べる
		} else if ( *pd == 0 ) {			//データが空
			return MATCHING_FALSE;				//キーが空ならTRUE、キーが空でないならFALSE
		}
		if (*pd == *pk) {					//データの1バイト目とキーの1バイト目が一致する
			pd++;
			pk++;
			continue;						//残りを使ってもう1ループする 
		} else {							//データの1バイト目とキーの1バイト目が一致しない
			return MATCHING_FALSE;			//データのみ1バイト進めて比べる
		}
	}
}