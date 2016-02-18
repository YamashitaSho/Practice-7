#include <stdio.h>
#include <string.h>
#include "header.h"

/*void input_a_line(char *inputline, char mode)
modeに応じてメッセージを出し、inputlineに入力された文字を入れる。

動作:
modeがMODE_LOADならばファイル切り替えのメッセージを出す。
modeがMODE_SEARCHならば検索ワード入力のメッセージを出す。
標準入力のバッファをフラッシュし、1行入力させる。
空行を入力された場合はそのまま改行が、
文字を入力された場合は改行を\0に置き換えてinputlineに出力する。
*/
void input_a_line(char *inputline, char mode){
	char input[256];
	char *newline;									//改行検出用
	
	if (mode == MODE_LOAD){
		printf("\n読み込むファイル名を入力してください。\n何も入力せずにEnterを押すと%sが読み込まれます。\n>>", FILENAME);
	} else if (mode == MODE_SEARCH) {
		printf("検索するワードを入力してください。\n>>");
	}
	fflush(stdin);
	fgets(input , 255 , stdin);
	
	if (input[0] != '\n') {
		newline = memchr(input , '\n', 255);		//fileの終端にある改行コードを検出する
		*newline = '\0';							//'\0'に置き換える
	}
	strncpy(inputline , input , 255);
}
