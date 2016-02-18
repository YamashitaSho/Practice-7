#include <stdio.h>
#include <dirent.h>
#include "header.h"

/* int ls_print(char *dir)
ディレクトリdirに含まれるファイル名を一覧表示する。

動作:
ディレクトリが空でないことを確認し、1ファイルごとに改行しつつファイル名を表示する。
ディレクトリが読み込めない場合はDIR_ERRORを、
正常に読み込めた場合はNO_ERRORを返す。
*/
int ls_print(char *dir){
	DIR *directory = opendir(dir) ;
	struct dirent *directoryp;
	
	if( directory == NULL ){
		return DIR_ERROR;
	}
	
	printf("%sフォルダ内のファイル一覧を表示します\n", dir);
	while ( (directoryp = readdir(directory)) != NULL ){
		printf("%s\n",directoryp->d_name);
	}
	return NO_ERROR;
}
