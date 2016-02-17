#include <stdio.h>
#include <string.h>
#include <dirent.h>

/*
C言語学習用課題
ファイルから文字列を取得し、構造体に格納する。

ファイル名	user.txt

ファイル内容の並びは「氏名」「かな」「電話番号」で構成。ファイルフォーマットは CSV.
1ファイル最大100名まで格納されている。

内容例
木村拓哉,きむらたくや,075-111-2221
稲垣五郎,いながきごろう,075-111-2222
香取慎吾,かとりしんご,075-111-2223

エラーチェック
ファイルが見つからない
ファイルフォーマットエラー
その他のエラー

コマンドライン
tcusers

*/
#define DATA_NUM 100			//読込データ数の上限
#define WORKING_DIR "data/"		//データフォルダ
#define FILENAME "tcusrs-00.csv"//デフォルトファイル名
#define DEFAULT_WORD "*"		//デフォルトの検索ワード(初期化用)

#define NO_ERROR 0				//エラーなし
#define FILE_ERROR 1			//ファイル読込のエラー
#define FILETYPE_ERROR 2		//ファイル形式のエラー
#define NO_WORD_ERROR 3			//検索ワード入力なし
#define DIR_ERROR 4				//ディレクトリが存在しない、または空
#define UNKNOWN_ERROR -1		//不明なエラー

#define MODE_SETUP '\0'			//起動処理
#define MODE_MENU '0'			//メニュー
#define MODE_LOAD '1' 			//ファイル読込
#define MODE_SEARCH '2'			//検索
#define MODE_FILELIST '3'		//ファイルリスト表示
#define MODE_PRINT '4'			//フォルダ内のファイル表示 デバッグ用
#define MODE_EXIT '9'			//終了

#define MATCHING_FALSE 0
#define MATCHING_TRUE 1

typedef struct _user user_t;
struct _user{
	char name[50];
	char ruby[50];
	char phone[20];
};

void struct_init(user_t *p);				//構造体配列の初期化(先頭要素のポインタを受け取る)
int main_loop(user_t *p ,char *file, char mode, int err);		//メニューを入力させて各処理に移行する
char get_menu();							//メニューを入力させる

int struct_load(user_t *p ,char *file, char mode);		//構造体にファイルをロードする
void input_a_line(char *file, char mode);	//ファイル名を入力させる

int ls_print(char *dir);						//dirの中身を表示する

void struct_print(user_t *p);				//構造体の内容を表示する
int struct_search(user_t *p);				//検索
int check(char *data, char *keyword);		//検索本体
void search_result(user_t *p, char *matching, int matchnumber);
											//検索結果を表示する

int error_msg(int err);						//エラーがある場合はエラーメッセージを出し、致命的なエラーは返り値にNO_ERROR以外を返す

//main関数-----------------------------------------------------------------------
int main(){
	char file[256] = FILENAME;
	char mode = MODE_SETUP;
	int err = NO_ERROR;
	
	user_t data[DATA_NUM];
	user_t *data_ptr;
	data_ptr = data;
	err = struct_load(data_ptr , file , mode);
//	printf("12000になればOK:%lu\n",sizeof(*data_ptr));
	//	memset(data_ptr , '\0', sizeof(*data_ptr));
	main_loop(data_ptr , file, mode, err);
	
	return 0 ;
}
//mainここまで--------------------------------------------------------------------

//構造体の配列を空にする(ポインタから参照したmemsetでは配列番号[0]しか空にならなかった)-----
//[0]を空にして残りに[0]を代入する---------------------------------------------------
void struct_init(user_t *data){
	int i = 0;
	memset(data->name,'\0',50);
	memset(data->ruby,'\0',50);
	memset(data->phone,'\0',20);
	for( i = 1; i < DATA_NUM ; i++){
		data[i] = data[0];
	}
}

//メインループ--------------------------------------------------------------------
int main_loop(user_t *data_ptr ,char *file, char mode, int err){
	char dir[] = WORKING_DIR;
	mode = MODE_MENU;
	
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
	return 0;
}

//メニュー表示・入力---------------------------------------------------------------
	
char get_menu(){
	char input[50];
	char mode;
	printf("%c.ファイル切替\n%c.検索\n%c.ファイル一覧表示\n%c.終了\nメニュー番号を入力してください:", MODE_LOAD , MODE_SEARCH , MODE_FILELIST , MODE_EXIT);
	fflush(stdin);
	fgets(input,50,stdin);
	fflush(stdin);
	mode = input[0];
	
	return mode;
}

//モードごとにメッセージを出しつつ1行入力させる-----------------------------------------
//何も入力されなかった場合、inputlineには改行コードが先頭に入っている
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

//FILENAMEのファイルを構造体pに読み込む----------------------------------------------
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
	(最大119バイト→120バイト目は必ず'\0'になっている→strtokでバッファオーバーフローする心配はない)
	この段階では読み込む行数に制限はかけない*/
		if (buf[0] == '\0'){					//bufに文字が入っていない場合格納終了
			break;
		}

		if (buf[0] != '\n'){							//1文字目が改行の場合、スキップ
			buf_p = strtok( buf , "," );			//もし","がいなくても119バイトで止まる
			if (buf_p != 0) {						//","が見つかっていた場合
				strncpy( (p+data_c)->name , buf_p ,49);	//buf_pが示す値を49バイトまでコピー(50バイトまで初期化してある)
			} else {								//見つからないならファイル形式エラー
				return FILETYPE_ERROR;
			}
			buf_p = strtok( NULL , "," );
			if (buf_p != 0) {
				strncpy( (p+data_c)->ruby , buf_p ,49);
			} else {
				return FILETYPE_ERROR;
			}
			buf_p = strtok( NULL , "\n" );			//構造体の3つ目の区切り文字は改行コード
			if (buf_p != 0){
				strncpy( (p+data_c)->phone , buf_p ,19);
			} else {
				return FILETYPE_ERROR;
			}
			data_c ++;

			if ( data_c >= DATA_NUM ){
				break;								//DATA＿NUM個受け取ったら終わり
			}
		}/* else {
			printf("行飛ばしだよ\n");
		}*/

		memset( buf , '\0' , 120 );					//一周ごとにbufを'\0'で初期化
	}
	
	fclose(fp);
	printf("ファイルを読み込みました:%s\n\n", work_file);
	return NO_ERROR;
}

//dataフォルダの内容を表示する-------------------------------------------------------
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

//構造体の内容を表示する------------------------------------------------------------
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

//構造体の内容から検索を行う---------------------------------------------------------
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
		
		if ( ( matching[i] = check((p+i)->name, keyword) ) == 0 ) {		//nameとkeywordを比べて、マッチしない
			if ( (matching[i] = check((p+i)->ruby, keyword) ) == 0 ) {	//rubyとkeywordを比べて、マッチしない
				matching[i] = check((p+i)->phone, keyword);							//phoneとkeywordを比べた結果を入れる
			}											//name,rubyと一致した場合はこの部分はMATCHING_TRUEが入っているはず
		}
		if (matching[i] != 0){						//TRUE
			matchnumber ++;
		}
		i++;
		if (i > DATA_NUM -1){						// DATA_NUM-1 番目までしか存在しない
			break;
		}
	}
	
//	printf("search:%s\n", keyword);										//検索ワード
//	for (int m=0;m<DATA_NUM;m++){printf("matching:%c\n",matching[m]);}	//マッチングデータの表示(長い)
	search_result( p , matching , matchnumber);
	return 0;
}

//検索本体-----------------------------------------------------------------------
//data内にkeywordがあればTRUEを、なければFALSEを返す。

int check(char *data, char *keyword){
	char *pd = data;
	char *pk = keyword;
	if (*pk == 0) {						//キーが空
		return MATCHING_TRUE;
	} else if ( *pk == '*' ){			//キーの先頭がアスタリスク
		return check( pd, (pk+1) );			//キーのみ1バイト進めて比べる
	} else if ( *pd == 0 ) {			//データが空
		return (*pk == 0);				//キーが空ならTRUE、キーが空でないならFALSE
	}
	if (*pd == *pk) {					//データの1バイト目とキーの1バイト目が一致する
		return check( (pd+1), (pk+1) );		//データもキーも1バイト進めて続きを比べる
	} else {							//データの1バイト目とキーの1バイト目が一致しない
		return check( (pd+1), pk );			//データのみ1バイト進めて比べる
	}
}

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


//エラーメッセージ-----------------------------------------------------------------
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