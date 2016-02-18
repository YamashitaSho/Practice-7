#include <string.h>
#include "header.h"

/* struct_init(user_t *data)
user_t構造体の変数配列dataを初期化する。

動作:
構造体dataの先頭ポインタを受け取り、
data[0]の三要素name、ruby、phoneを\0で初期化。
[1]から[DATA＿NUM-1]まで[0]を代入操作する。
*/

void struct_init(user_t *data){
	int i = 0;
	memset(data->name,'\0',NAME_SIZE);
	memset(data->ruby,'\0',RUBY_SIZE);
	memset(data->phone,'\0',PHONE_SIZE);
	for( i = 1; i <= (DATA_NUM-1) ; i++){
		data[i] = data[0];
	}
}
