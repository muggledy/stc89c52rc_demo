#ifndef __ARRAY_POOL_H__
#define __ARRAY_POOL_H__

#include "global.h"

//type must be a typedef name, and data must be the first field
#define INIT_POOL(name, type, _size) \
struct pool_##type {\
	type data;\
	uint8_t valid;\
	int32_t pos;\
};\
struct pool_##type _##name[_size] = {0};\
struct head_pool_##type {\
	struct pool_##type *pool;\
	uint32_t size;\
	int32_t pos;\
	type *item_ptr;\
	uint32_t valid_num;\
	uint8_t op_code;\
};\
struct head_pool_##type name = { _##name, _size};

#define INSERT_POOL(pool, item) {\
	pool.op_code = 1;\
	pool.pos = pool.size - 1;\
	pool.item_ptr = NULL;\
	while(pool.pos >= 0) {\
		if (!pool.pool[pool.pos].valid) {\
			pool.op_code = 0;\
			pool.pool[pool.pos].data = item;\
			pool.pool[pool.pos].pos = pool.pos;\
			pool.pool[pool.pos].valid = 1;\
			pool.item_ptr = &(pool.pool[pool.pos].data);\
			pool.valid_num++;\
			break;\
		}\
		pool.pos--;\
	}\
}

#define GET_LEN(pool) (pool.valid_num)
#define GET_CAPACITY(pool) (pool.size)

//pos can be [0, GET_CAPACITY(pool)) 
#define GET_POOL_ITEM_BY_POS(pool, _pos) {\
	pool.op_code = 1;\
	if ((_pos >=0) && (_pos < pool.size) && pool.pool[_pos].valid) {\
		pool.op_code = 0;\
		pool.item_ptr = &(pool.pool[_pos].data);\
		pool.pos = _pos;\
	} else {\
		pool.item_ptr = NULL;\
		pool.pos = -1;\
	}\
}

//get one valid item arbitrarily 
#define GET_POOL_ITEM(pool) {\
	if (0 == pool.valid_num) {\
		pool.pos = -1;\
		pool.op_code = 1;\
		pool.item_ptr = NULL;\
	} else {\
		pool.pos = 0;\
		while(pool.pos < pool.size) {\
			if (pool.pool[pool.pos].valid) {\
				pool.op_code = 0;\
				pool.item_ptr = &(pool.pool[pool.pos].data);\
				break;\
			}\
			pool.pos++;\
		}\
	}\
}

//valid for insert and get operation, if can't get valid item, GOT_ITEM_PTR() return NULL
#define GOT_ITEM_PTR(pool) (pool.item_ptr)

#define DEL_POOL_ITEM_BY_POS(pool, _pos) {\
	pool.op_code = 1;\
	if ((_pos >=0) && (_pos < pool.size)) {\
		pool.op_code = 0;\
		pool.pos = _pos;\
        if (pool.pool[_pos].valid) {\
            pool.valid_num--;\
        }\
		pool.pool[_pos].valid = 0;\
		pool.item_ptr = NULL;\
	}\
}

#define DEL_POOL_ITEM(pool, type, _item_ptr) {\
	if (((struct pool_##type *)(_item_ptr))->valid) {\
		pool.valid_num--;\
	}\
	pool.pos = ((struct pool_##type *)(_item_ptr))->pos;\
	((struct pool_##type *)(_item_ptr))->valid = 0;\
	pool.item_ptr = NULL;\
	pool.op_code = 0;\
}

#define TST_ITEM_VALID_BY_POS(pool, pos) (pool.pool[pos].valid)
#define TST_ITEM_VALID(type, item_ptr) (((struct pool_##type *)(item_ptr))->valid)
#define GET_POS_OF_ITEM(type, item_ptr) (((struct pool_##type *)(item_ptr))->pos)

#define CLEAR_POOL(pool) {\
	pool.pos = pool.size - 1;\
	while(pool.pos >= 0) {\
		pool.pool[pool.pos].valid = 0;\
		pool.pos--;\
	}\
	pool.op_code = 0;\
	pool.item_ptr = NULL;\
	pool.pos = 0;\
	pool.valid_num = 0;\
}

#endif