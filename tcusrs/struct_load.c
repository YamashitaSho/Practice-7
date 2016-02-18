#include <stdio.h>
#include <string.h>
#include "header.h"

//FILENAMEのファイルを構造体pに読み込む
int struct_load(user_t *p ,char *file, char mode){
	int data_c = 0;			//ループ内でのデータ数カウント
	char buf[120];
	char work_file[256+strlen(WORKING_DIR)];
												//ディレクトリ名も考慮に入れて配列を確保、ディレクトリ名を入れておく
	char *buf_p;
	FILE *fp;
	
	struct_init(p);
	memset( work_file , '\0' , 256+strlen(WORKING_DIR) );
	strncpy( work_file, WORKING_DIR, strlen(WORKING_DIR) );
	memset( buf , '\0' , 120 );					//buf[119]まで'\0'埋め
	
	if (mode ==	MODE_SETUP){					//起動処理か
		strncat(work_file, FILENAME, 255);			//デフォルトファイル名連結
	} else {
		input_a_line(file , MODE_LOAD);			//起動処理でないならファイル名をユーザーに要求する
		if ( file[0] == '\n' ) {				//無入力でエンターを押された
			strncat(work_file, FILENAME, 255);		//デフォルトファイル名連結
		} else {
			strncat(work_file, file, 255);			//フォルダ名に連結する
		}
	}
		
	fp = fopen(work_file,"r");
	if (fp == NULL){
		printf("%s:ファイルが開けませんでした。\n", work_file);
		fclose(fp);
		return FILE_ERROR;
	}
	
	while(fgets(buf , 119 , fp) != NULL){		/*NULLまで1行ごとに読み込む
	(最大119バイト→120バイト目は必ず'\0'になっている→strtokでバッファオーバーフローする心配はない)*/
		if ( (buf[0] == '\0') || ( data_c == DATA_NUM ) ) {
			break;									//bufに文字が入っていない または すでにDATA_NUM個受け取っている場合格納終了
		} else if ( buf[0] == '\n') {
			data_c ++;
			continue;								//1文字目が改行の場合、スキップ
		}
		
		buf_p = strtok( buf , "," );			//もし","がいなくても119バイトで止まる
		if (buf_p != 0) {						//","が見つかっていた場合
			strncpy( (p+data_c)->name , buf_p , NAME_SIZE - 1 );	//buf_pが示す値をNAMESIZE-1バイトコピー
		} else {								//見つからないならファイル形式エラー
			return FILETYPE_ERROR;
		}
		buf_p = strtok( NULL , "," );
		if (buf_p != 0) {
//			strncpy( (p+data_c)->ruby , buf_p , RUBY_SIZE ｰ 1 );
		} else {
			return FILETYPE_ERROR;
		}
		buf_p = strtok( NULL , "\n" );			//構造体の3つ目の区切り文字は改行コード
		if (buf_p != 0){
			strncpy( (p+data_c)->phone , buf_p , PHONE_SIZE - 1);
		} else {
			return FILETYPE_ERROR;
		}
		data_c ++;
		memset( buf , '\0' , 120 );					//一周ごとにbufを'\0'で初期化
	}
	fclose(fp);
	printf("ファイルを読み込みました:%s\n", work_file);
		
	return NO_ERROR;
}
