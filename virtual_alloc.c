#include "virtual_alloc.h"

#define GET_BIT(byte, bit) ((byte>>bit)&1)
#define SET_BIT(byte, bit) (byte |= (1 << bit))
#define CLR_BIT(byte, bit) (byte &= ~(1 << bit))
#define MAX_INIT 64

/*
 * Description: This is a function for calculating how many nodes in 
 	a tree have (level_num) level.
 	Return: the number of node in this tree 
 * */
int calculate_node(uint8_t level_num){
	int ret = 0;
	for(int i = 0; i < level_num; ++i){
		ret += (int)pow(2,i);
	}
	return ret;
}

/*
 * Description: This is a function for calculating the size is 2 to 
 	the which value. if the calculate value is less than min_size, 
 	return min_size, otherwise, return that value.
 	Return: the value or the min_size.
 * */
int calculate_size(uint32_t size, uint8_t min_size){
	int ret = 0;
	while(size!=0){
		size/=2;
		ret +=1;
	}
	if(ret<min_size){
		ret = min_size;
	}
	return ret;
}

/*
 * Description: This is a function for checking if the ancestors of a
 	node are all zero or for checking if the ancestors of a node have
 	at least a one.
 	Return: 1 if there is a 1, 0 if all are 0
 * */
int if_all_ancestor_zero(void * heapstart, uint8_t padding, int i){
	int level = (int)ceil((i+padding)/(double)8);
	uint8_t* cur = (uint8_t*)(heapstart + 2 - 1 + level);
	// base case: return itself
	if(i == 1){
		return GET_BIT(*cur, (level*8-i-padding));
	}
	// recur: return itself and its father
	return GET_BIT(*cur, (level*8-i-padding)) ||
	 if_all_ancestor_zero(heapstart, padding, i/2);
}

/*
 * Description: This is a function for checking if the offsprings of a
 	node are all zero or for checking if the offsprings of a node have
 	at least a one.
 	Return: 1 if there is a 1, 0 if all are 0
 * */
int if_all_offspring_zero(void * heapstart, uint8_t padding, 
	int i, int times){
	int level = (int)ceil((i+padding)/(double)8);
	uint8_t* cur = (uint8_t*)(heapstart + 2 - 1 + level);
	// base case: return itself
	if(times == 0){
		return GET_BIT(*cur, (level*8-i-padding));
	}
	// recur: return itself and its offsprings
	return GET_BIT(*cur, (level*8-i-padding)) || 
	if_all_offspring_zero(heapstart, padding, i*2, times-1) ||
	if_all_offspring_zero(heapstart, padding, i*2+1, times-1);
}

/*
 * Description: This is a function for print the info of the heap.
 	(post-order traversal)
 * */
void print_free_allocated(void * heapstart, uint8_t padding, 
	int i, int cur_size){
	uint8_t min_size = *(uint8_t*)(heapstart + 1);
	int level = (int)ceil((i+padding)/(double)8);
    uint8_t* cur = (uint8_t*)(heapstart + 2 - 1 + level);
    /* if the node is the first node, which is the root of the tree
	 * if this node is 0(free), check if all offspring are 0, if true
	 	print free this node.
	 * if this node is 1(allocated), print allocated this node.
	*/
    if(i == 1){
    	if(GET_BIT(*cur, (level*8-i-padding)) == 0){
    		if(!if_all_offspring_zero(heapstart, padding, i, 
    			cur_size-min_size)){
	    		printf("free %d\n", (int)pow(2, cur_size));
	    	}
    	}else if(GET_BIT(*cur, (level*8-i-padding)) == 1){
    		printf("allocated %d\n", (int)pow(2, cur_size));
    	}
    	return;
    }
    /* if the node is even number, which is the left child of a node.
	 * if this node is 0(free), check if all offspring are 0 and its 
	 	brother or brother's offspring have 1. If true, print free 
	 	this node.
	 * if this node is 1(allocated), print allocated this node.
	*/
	if(i%2==0){
		if(GET_BIT(*cur, (level*8-i-padding)) == 0){
			if(if_all_offspring_zero(heapstart, padding, i+1, 
			cur_size-min_size) &&
				!if_all_offspring_zero(heapstart, padding, i, 
			cur_size-min_size)){
				printf("free %d\n", (int)pow(2, cur_size));
			}
		}
		else if(GET_BIT(*cur, (level*8-i-padding)) == 1){
			printf("allocated %d\n", (int)pow(2, cur_size));
		}
	/* if the node is odd number, which is the right child of a node.
	 * if this node is 0(free), check if all offspring are 0 and its 
	 	brother or brother's offspring have 1. If true, print free 
	 	this node.
	 * if this node is 1(allocated), print allocated this node.
	 * Do the recursion for post-order traversal
	*/
	}else if(i%2==1){
		if(GET_BIT(*cur, (level*8-i-padding)) == 0){
			if(if_all_offspring_zero(heapstart, padding, i-1, 
			cur_size-min_size) &&
				!if_all_offspring_zero(heapstart, padding, i, 
			cur_size-min_size)){
				printf("free %d\n", (int)pow(2, cur_size));
			}
		}
		else if(GET_BIT(*cur, (level*8-i-padding)) == 1){
			printf("allocated %d\n", (int)pow(2, cur_size));
		}
		print_free_allocated(heapstart, padding, i/2, cur_size+1);
    }
}

/*
 * Description: This is a function for initialize the allocator
 * */
void init_allocator(void * heapstart, uint8_t initial_size, uint8_t min_size){
	if(heapstart == NULL){
		return;
	}
	if(initial_size > MAX_INIT){
    	return;
    }
    if(min_size > initial_size){
    	return;
    }
    int byte_num= (int)ceil(calculate_node(initial_size-min_size+1)/(double)8);

    // sbrk the size of (2+DS)
    virtual_sbrk(sizeof(uint8_t)*(2 + byte_num));
    *(uint8_t*)(heapstart + 0) = initial_size;
	*(uint8_t*)(heapstart + 1) = min_size;
	for(int i = 0; i < byte_num; ++i){
		*(uint8_t*)(heapstart + 2 + i) = 0;
	}
	// sbrk the size of the heap except the DS and 2.
	virtual_sbrk((uint32_t)pow(2, initial_size));
}

/*
 * Description: This is a function for malloc a space
 * Return: A pointer of the heap means malloc success, 
 		NULL means malloc fails.
 * */
void * virtual_malloc(void * heapstart, uint32_t size) {
	if(heapstart == NULL){
		return NULL;
	}
	uint8_t initial_size = *(uint8_t*)(heapstart + 0);
	uint8_t min_size = *(uint8_t*)(heapstart + 1);
	int byte_num= (int)ceil(calculate_node(initial_size-min_size+1)/(double)8);
	if(size== 0){
		return NULL;
	}
    if(calculate_size(size-1, min_size) > initial_size){
    	return NULL;
    }
    uint8_t padding = 8 - calculate_node(initial_size-min_size+1)%8;
    int start = calculate_node(initial_size
    	-calculate_size(size-1, min_size))+1;
    int finish = calculate_node(initial_size
    	-calculate_size(size-1, min_size)+1);
    uint8_t found = 0;
    uint32_t first_zero = 0;
    int i;
    // find the level which fit to the size
    for(i = start; i <= finish; ++i){
    	int level = (int)ceil((i+padding)/(double)8);
    	uint8_t* cur = (uint8_t*)(heapstart + 2 - 1 + level);
    	// check if this place is 0
    	if(GET_BIT(*cur, (level*8-i-padding)) == 0){
    		// check if this place's ancestors are 0
    		if(!if_all_ancestor_zero(heapstart, padding, i)){
    			// check if this place's offspring are 0
    			if(!if_all_offspring_zero(heapstart, padding, i, 
    				calculate_size(size-1, min_size)-min_size)){
    				// use a var to record the first free place
    				if(first_zero == 0){
    					first_zero = i;
    				}
    				/* if the cur place is even, check its right brother or
						right brother's offspring have 1, if true, set this
						place to 1. break.
					*/
    				if(i%2==0){
    					if(if_all_offspring_zero(heapstart, padding, i+1, 
			calculate_size(size-1, min_size)-min_size)){
    						SET_BIT(*cur, (level*8-i-padding));
	    					found = 1;
	    					break;
    					}
    				/* if the cur place is odd, check its left brother or
						left brother's offspring have 1, if true, set this
						place to 1. break.
					*/
    				}else if(i%2==1){
    					if(if_all_offspring_zero(heapstart, padding, i-1, 
			calculate_size(size-1, min_size)-min_size)){
    						SET_BIT(*cur, (level*8-i-padding));
	    					found = 1;
	    					break;
    					}
    				}
	    			
	    		}
    		}
    	}
    }

    /* if we did not find the space and we find the first place,
     we set the first place to 1
    */
    if (found == 0 && first_zero!=0){
    	i = first_zero;
    	int level = (int)ceil((i+padding)/(double)8);
    	uint8_t* cur = (uint8_t*)(heapstart + 2 - 1 + level);
    	SET_BIT(*cur, (level*8-i-padding));
    }
    /* if we did not find the space and we did not find the first place,
     means there is no suitable place. Return NULL.
    */
    if (found == 0 && first_zero==0){
    	return NULL;
    }
    // return the counted address
    return (heapstart + 2 + byte_num + (i-start) * 
    	(uint32_t)pow(2, calculate_size(size-1, min_size)));
}

/*
 * Description: This is a function for free a space
 * Return: 0 if success, 1 if fail
 * */
int virtual_free(void * heapstart, void * ptr) {
	if(heapstart == NULL){
		return 1;
	}
	// if ptr is null, fail
	if(ptr == NULL){
		return 1;
	}
	uint8_t initial_size = *(uint8_t*)(heapstart + 0);
	uint8_t min_size = *(uint8_t*)(heapstart + 1);
	int byte_num= (int)ceil(calculate_node(initial_size-min_size+1)/(double)8);
	uint8_t cur_size = initial_size;
    uint8_t padding = 8 - calculate_node(initial_size-min_size+1)%8;
    // if the ptr is at the wrong place, fail
	if(ptr - heapstart - 2 - byte_num < 0){
		return 1;
	}
	// search from top to bottom
    while(cur_size>=min_size){
    	// check if the pow(2,cur_size) can be divided by ptr place
    	if((ptr - heapstart - 2 - byte_num)%(uint32_t)pow(2,cur_size)==0){
    		int start = calculate_node(initial_size-cur_size)+1;
    		uint32_t cur_place = (start+((ptr - heapstart - 2 - byte_num)
    			/(uint32_t)pow(2,cur_size)));
    		int level = (int)ceil((cur_place+padding)/(double)8);
	    	uint8_t* cur = (uint8_t*)(heapstart + 2 - 1 + level);
	    	// check if the place of this level is 1, if it is, change to 0
    		if(GET_BIT(*cur, (level*8-cur_place-padding)) == 1){
    			CLR_BIT(*cur, (level*8-cur_place-padding));
    			return 0;
    		}
    	}
	    cur_size--;
	}
	// if can not find, return 1
	return 1;
}

/*
 * Description: This is a function for realloc a space
 * Return: A pointer of the heap means realloc success, 
 		NULL means realloc fails.
 * */
void * virtual_realloc(void * heapstart, void * ptr, uint32_t size) {
	if(heapstart == NULL){
		return NULL;
	}
	// get the ptr size
	uint32_t ptr_size;
	uint8_t initial_size = *(uint8_t*)(heapstart + 0);
	uint8_t min_size = *(uint8_t*)(heapstart + 1);
	if(size>0){
		if(calculate_size(size-1, min_size) > initial_size){
	    	return NULL;
	    }
	}
	int byte_num= (int)ceil(calculate_node(initial_size-min_size+1)/(double)8);
	uint8_t cur_size = initial_size;
    uint8_t padding = 8 - calculate_node(initial_size-min_size+1)%8;
    int start;
    int level;
    uint8_t* cur;
    while(cur_size>=min_size){
    	if((ptr - heapstart - 2 - byte_num)%(uint32_t)pow(2,cur_size)==0){
    		start = calculate_node(initial_size-cur_size)+1;
    		level = (int)ceil(((start+
    			((ptr - heapstart - 2 - byte_num)/(uint32_t)pow(2,cur_size)))
    			+padding)/(double)8);
	    	cur = (uint8_t*)(heapstart + 2 - 1 + level);
    		if(GET_BIT(*cur, (level*8-(start+
    			((ptr - heapstart - 2 - byte_num)/(uint32_t)pow(2,cur_size)))
    			-padding)) == 1){
    			ptr_size = (uint32_t)pow(2,cur_size);
    			break;
    		}
    	}
	    cur_size--;
	}

	// if ptr is null and size if not 0, we do the malloc
	if(ptr == NULL && size != 0){
		return virtual_malloc(heapstart, size);
	}
	// if size is 0, we do the free and return NULL 
	if(size == 0){
		virtual_free(heapstart, ptr);
		return NULL;
	}
	// free the place first and do the malloc
	virtual_free(heapstart, ptr);
	void * ret = virtual_malloc(heapstart, size);
	// if malloc fails, change it back to the status before free
	// and return NULL
	if(ret== NULL){
		SET_BIT(*cur, (level*8-(start+((ptr - heapstart - 2 - byte_num)
    				/(uint32_t)pow(2,cur_size)))-padding));
		return ret;
	}
	if(ptr_size >= size){
		memmove(ret, ptr, size);
	}else{
		memmove(ret, ptr, ptr_size);
	}
    return ret;
}

/*
 * Description: This is a function for print the info of heap
 * */
void virtual_info(void * heapstart) {
	if(heapstart == NULL){
		return;
	}
	uint8_t initial_size = *(uint8_t*)(heapstart + 0);
	uint8_t min_size = *(uint8_t*)(heapstart + 1);
    uint8_t cur_size = min_size;
    uint8_t padding = 8 - calculate_node(initial_size-min_size+1)%8;
    int start = calculate_node(initial_size-cur_size)+1;
    int finish = calculate_node(initial_size-cur_size+1);
    for(int i = start; i <= finish; ++i){
 		print_free_allocated(heapstart, padding, i, cur_size);
    }
}
