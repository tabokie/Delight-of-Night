#include <stdio.h>
#include <stdlib.h>


//arrangement in a static memory block

typedef struct initvector{
	//memory unit
	void* block_head;
	void* block_end;
	int block_len;
	
	//operating unit 
	int type_len;
	int us_len;
	int us_idx;//from 0
	void* head;
	void* tail;
}vector;

#define vector(type,vec_name,num) ({\
	int vector_ret = 0;\
	if(num > 0){\
		vector vec_name = (vector)malloc(sizeof(vector));\
		vec_block.block_len = sizeof(type)*(num+1);\
		vec_name.block_head = (type*)malloc(vec_block.block_len);\
		vec_name.block_end = (type*)(vec_name.block_head + num);\
		vec_name.type_len = sizeof(type);\
		vec_name.us_len = 0;\
		vec_name.us_idx = -1;\
		vec_name.head = NULL;\
		vec_name.tail = NULL;\
		vector_ret = 1;\
	}\
	vector_ret ;\
})

#define insert_item(vec , idx , item)({\
	int insert_ret = 0;\
	if(vec.us_len + type_len * 2 <= vec.block_len){\
		int tdx = vec.us_idx;\
		insert_ret = 1;\
		if(idx == tdx + 1 ){\
			vec.us_len += vec.type_len;\
			vec.us_idx ++;\
			vec.tail ++;\
			*vec_tail = item;\
		}\
		else if (idx == 0){\
			vec.us_len += vec.type_len;\
			vec.us_idx ++;\
			if(vec.head == vec.block_head){\
				vec.head = vec.block_end - 1;\
				*vec.head = item;\
			}\
			else{\
				vec_head--;\
				*vec.head = item;\
			}\
		}\
		else if (idx < tdx){\
			vec.us_len += vec.type_len;\
			vec.us_idx ++;\
			int bklen = (vec.tail - vec.block_head)/vec.type_len + 1;\
			if(idx > tdx - bklen){\
				void* p = vec_tail +1;\
				int i = 0;\
				while(i >= idx){\
					*p = *(vec_tail - i);\
					p--;\
					i++;\
				}\
				*(vec_tail - i) = item;\
				vec_tail++;\
			}\
			else{\
				void* p = vec_head - 1;\
				int i = 0;\
				while(i < idx){\
					*p = *(vec_head+i);\
					p++;\
					i++;\
				}\
				*(vec_head+i) = item;\
				vec_head--;\
			}\
		}\
		else{\
			insert_ret = 0;\
		}\
	}\
	insert_ret;\
})

void get_item(vector vec, int idx){
	if (idx >= 0 && idx <= vec.us_idx){
		void* p = vec.head;
		for (int i = 1; i <= idx; i++){
			if ((int*)vec.block_end - (int*)p >= 2 * vec.type_len){
				p++;
			}
		}
	}
	else{
		return NULL;
	}
}