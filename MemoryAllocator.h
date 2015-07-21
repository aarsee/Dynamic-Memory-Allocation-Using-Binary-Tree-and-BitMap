#ifndef	MEMORY_ALLOCATOR_H
#define	MEMORY_ALLOCATOR_H

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned char byte;
typedef	unsigned char boolean;
typedef int int32;
typedef short int16;
typedef char int8;

#define	GUARD_BYTES	20
#define	N_BYTES 4

#define	FREE_CHUNK 0
#define	ALLOCATED_CHUNK	1

#define	TRUE 1
#define	FALSE 0

typedef	struct	_MemoryChunk
{
	uint32	size:30;
	uint32	flag:1;
	uint32	avl_flg:1;
	
	struct	_MemoryChunk *left;
	struct	_MemoryChunk *right;
	struct	_MemoryChunk *parent;
}MemoryChunk;

MemoryChunk* initialize_memory_chunk(void *ptr,uint32 size);
uint32	high_bytes(uint32 size);
uint32	low_bytes(uint32 size);
uint32	_guard_bytes();
uint32	total_size();
boolean	is_chunk_size_sufficient(uint32 size);
boolean	divide_chunk(MemoryChunk *chunk,uint32 desired_size,MemoryChunk	**left,MemoryChunk **right);
void insert_footer(MemoryChunk *chunk);
uint32*	read_footer(MemoryChunk *chunk);
void* get_requested_memory(MemoryChunk *chunk);

typedef	void(*tree_search_callback)(MemoryChunk*,uint32 *flag,uint32 desired_size);
typedef	struct	_BinaryTree
{
	MemoryChunk	*root;
	uint32	totalChunks;
	uint32	right_depth;
	uint32	left_depth;
	uint32	right_element_count;
	uint32	left_element_count;
	uint32	right_leaf_count;
	uint32	left_leaf_count;
}BinaryTree;

void insert_chunk(BinaryTree *tree,MemoryChunk *chunk);
void remove_chunk(BinaryTree *tree,MemoryChunk *chunk);
boolean	initialize_binary_tree(BinaryTree *tree);
MemoryChunk* get_closet_chunk(BinaryTree *tree,uint32 size,tree_search_callback search_fn);
MemoryChunk* get_bigger_chunk(BinaryTree *tree,uint32 size,tree_search_callback search_fn);

typedef	struct	_MemoryPool
{
	void	*mem_pool;
	uint32	mem_pool_size;
	MemoryChunk	*largestChunk;
	BinaryTree	freeChunkTree;
	BinaryTree	allocatedChunkTree;
}MemoryPool;


MemoryPool*	create_memory_pool(void	*ptr,uint32	size);
void* allocate_best_fit(MemoryPool *pool,uint32 size);
void* allocate_worst_fit(MemoryPool *pool,uint32 size);
void deallocate(MemoryPool *pool,void	*ptr);

MemoryChunk* coalesce(MemoryPool* pool,MemoryChunk *chunk);

#endif	//MEMORY_ALLOCATOR_H