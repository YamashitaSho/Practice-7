#include <stdio.h>
#include <string.h>
#include "header.h"

/*char get_menu()
メニューを表示し、入力された文字の1バイト目を返す。

動作:
メニューを表示する。
文字入力を要求する前後に標準入力のバッファをフラッシュしている。
*/
	
char get_menu(){
	char input[256];
	char mode;
	
	memset(input , '\0' , 256);
	
	printf("%c.ファイル切替\n%c.検索\n%c.ファイル一覧表示\n%c.終了\nメニュー番号を入力してください:", MODE_LOAD , MODE_SEARCH , MODE_FILELIST , MODE_EXIT);
	
	fflush(stdin);
	fgets(input,255,stdin);
	fflush(stdin);
	
	mode = input[0];
	
	return mode;
}
