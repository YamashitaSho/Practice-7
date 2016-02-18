#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "header.h"

#include "check.c"
#include "error_msg.c"
#include "get_menu.c"
#include "input_a_line.c"
#include "ls_print.c"
#include "search_result.c"
#include "struct_init.c"
#include "struct_load.c"
#include "struct_print.c"
#include "struct_search.c"

/*
C言語学習用課題 #7
*/

int main(){
	char file[256] = FILENAME;
	char mode = MODE_SETUP;
	char dir[] = WORKING_DIR;
	int err = NO_ERROR;
	
	user_t data[DATA_NUM];
	user_t *data_ptr;
	data_ptr = data;
	
	err = struct_load(data_ptr , file , mode);	//デフォルトファイル読み込み
//main_loop関数はmainに統合
	while(1){
		if (error_msg(err) != NO_ERROR) {		//エラー終了
			break;
		}
		mode = get_menu();
		if ( mode == MODE_LOAD ){				//読み込みモード
			err = struct_load(data_ptr , file, mode);
		}
		if ( mode == MODE_SEARCH ){				//検索モード
			err = struct_search(data_ptr);			//検索
		}
		if ( mode == MODE_FILELIST ){
			err = ls_print(dir);
		}
		if ( mode == MODE_PRINT ){				//構造体の内容を表示する
			struct_print(data_ptr);
		}
		if ( mode == MODE_EXIT ){				//終了
			break;
		}
	}
	return 0 ;
}
