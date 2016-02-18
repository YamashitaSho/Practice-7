#include <stdio.h>
#include "header.h"

/* error_msg(int err)
errに応じてメッセージを表示し、処理を分岐する。
通常のエラーの場合はNO_ERRORを返し、不明なエラーについてはUNKNOWN_ERRORを返す。

動作:
エラーコードerrを受け取る。
エラーがない場合は何もせずエラーコードNO_ERRORを返す。

ファイル読込エラーが発生した場合
ファイル形式エラーが発生した場合
検索ワードが何も入力されていない場合
フォルダの内容が表示できない場合
はエラーメッセージを表示し、エラーコードNO_ERRORを返す。

どれにも当てはまらないerrを受け取った場合はエラーメッセージを表示し、UNKNOWN_ERRORを返す。
*/
int error_msg(int err){
	switch (err){
	  case NO_ERROR:
		break;
		
	  case FILE_ERROR:						//ファイルが見つからない(探したファイルは表示済み)
		printf("Enterを押すとメインメニューに戻ります。\n");
		getc(stdin);
		fflush(stdin);
		break;
		
	  case FILETYPE_ERROR:
		printf("ファイル形式エラーです。\nEnterを押すとメインメニューに戻ります。\n");
		getc(stdin);
		fflush(stdin);
		break;
		
	  case NO_WORD_ERROR:
		printf("検索ワードを入力してください。\nEnterを押すとメインメニューに戻ります。\n");
		getc(stdin);
		fflush(stdin);
		break;
		
	  case DIR_ERROR:
		printf("フォルダの内容を表示できません。\nEnterを押すとメインメニューに戻ります。\n");
		getc(stdin);
		fflush(stdin);
		break;
		
	  default:
		printf("不明なエラーが発生しました。\n");
		return UNKNOWN_ERROR;
	}
	return NO_ERROR;
}