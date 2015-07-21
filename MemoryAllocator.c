#include "MemoryAllocator.h"

#define	GET_FLAG(x) ((x&0x80000000)>>31)
#define	GET_SIZE(x) (x&0x8FFFFFFF)
#define NULL ((MemoryChunk*)0)

uint32 count=0;

enum TRAVERSE_DIRECTION
{
	DOWN_RIGHT,
	DOWN_LEFT,
	UP_RIGHT,
	UP_LEFT
};

boolean	initialize_binary_tree(BinaryTree *tree)
{
	if(!tree)
	{
		return FALSE;
	}
	tree->root = 0;
	tree->totalChunks = 0;
	tree->left_depth = 0;
	tree->right_depth = 0;
	tree->left_element_count = 0;
	tree->right_element_count = 0;
	tree->left_leaf_count = 0;
	tree->right_leaf_count = 0;
	return TRUE;
}

void insert_chunk(BinaryTree *tree,MemoryChunk *chunk)
{
	MemoryChunk	*node,*parent;
	uint32	insertion_depth;
	if(tree == 0 || chunk == 0)
	{
		return;
	}
	insertion_depth = 0;
	node = tree->root;
	parent = 0;
	while(node)
	{
		parent = node;
		if(GET_SIZE(chunk->size) > GET_SIZE(node->size))
		{
			node = node->right;
		}
		else
		{
			node = node->left;
		}
		insertion_depth++;
	}
	if(parent)
	{
		if(GET_SIZE(chunk->size) > GET_SIZE(parent->size))
		{
			parent->right = chunk;
		}
		else
		{
			parent->left = chunk;
		}
		chunk->left = 0;
		chunk->right = 0;
		chunk->parent = parent;
		tree->totalChunks ++;
	}
	else
	{	
		tree->root = chunk;
		chunk->left = 0;
		chunk->right = 0;
		chunk->parent = 0;
		tree->totalChunks = 1;
	}
	if(tree->totalChunks > 1)
	{
		if(GET_SIZE(chunk->size) > GET_SIZE(tree->root->size))
		{
			if(insertion_depth > tree->right_depth)
			{
				tree->right_depth = insertion_depth;
				tree->right_leaf_count = 1;
			}
			else if(insertion_depth == tree->right_depth)
			{
				tree->right_leaf_count++;
			}
		}
		else
		{
			if(insertion_depth > tree->left_depth)
			{
				tree->left_depth = insertion_depth;
				tree->left_leaf_count = 1;
			}
			else if(insertion_depth == tree->left_depth)
			{
				tree->left_leaf_count++;
			}
		}
	}
}

void remove_chunk(BinaryTree	*tree,MemoryChunk	*chunk)
{
	MemoryChunk	*left,*right,*parent,*newNode;
	boolean	leafNode;
	if(tree->root == 0 || tree->totalChunks == 0 || chunk == 0)
	{
		return;
	}
	left	=	chunk->left;
	right	=	chunk->right;
	parent	=	chunk->parent;
	newNode	=	0;
	if(!chunk->left && !chunk->right)
	{
		leafNode = TRUE;
	}
	else
	{
		leafNode = FALSE;
	}

	if(right == 0)
	{
		newNode = left;
	}
	else
	{
		newNode	=	right;
	}

	if(left && right)
	{
		MemoryChunk	*ptr,*prev;
		ptr = right;
		prev = 0;
		while(ptr)
		{
			prev = ptr;
			ptr = ptr->left;
		}
		prev->left = left;
		left->parent = prev;
	}

	if(parent)
	{
		if(GET_SIZE(chunk->size) > GET_SIZE(parent->size))
		{
			parent->right = newNode;
		}
		else
		{
			parent->left = newNode;
		}
		if(newNode)
		{
			newNode->parent = parent;
		}
	}
	else
	{
		tree->root = newNode;
		if(newNode)
		{
			newNode->parent = 0;
		}
	}
	tree->totalChunks--;
	if(leafNode && tree->root)
	{
		if(GET_SIZE(chunk->size) > GET_SIZE(tree->root->size))
		{
			tree->right_leaf_count--;
		}
		else
		{
			tree->right_leaf_count--;
		}
	}
}

void traverse(BinaryTree *tree){
	traverse_binary_tree(tree->root);
}

void traverse_binary_tree(MemoryChunk *node)
{
		if(node!=0)
		{	
			printf("Value : %d\n",node->size);
		}
		else
		{
			printf("No root found in tree\n" );
		}
		if(node->left==0 && node->right==0)
		{
			//printf("No node found\n");
		}
		else if(node->left!=0 && node->right==0)
		{
	  		traverse_binary_tree(node->left);
	  	}
	  	else if(node->right!=0 && node->left==0)
	  	{	  
      		traverse_binary_tree(node->right);
      	}	
      	else
      	{
      		traverse_binary_tree(node->left);
      		traverse_binary_tree(node->right);
      	}	
}

MemoryChunk* get_closet_chunk(BinaryTree *tree,uint32 size,tree_search_callback search_fn)
{
	MemoryChunk	*node;
	MemoryChunk *parent;
	MemoryChunk *temp;
	temp = NULL;	
	if(tree == 0 || tree->root == 0 || search_fn == 0)
	{
		return 0;
	}
	node = tree->root;
	parent = node;
	while(node){
		parent = node;
	if(size > GET_SIZE(node->size)){
		node = node-> right;
	}
	else
		node = node->left;
	}
	if(GET_SIZE(parent->size) < size){
		printf("No block of sufficient size\n");
		return temp;
	}
	return parent;
}

MemoryChunk* get_bigger_chunk(BinaryTree *tree,uint32 size,tree_search_callback search_fn)
{
	MemoryChunk	*chunk;
	MemoryChunk	*node;
	MemoryChunk	*n;
	if(tree == 0 || tree->root == 0 || search_fn == 0)
	{
		return 0;
	}	
	chunk	=	0;
	node = tree->root;
	while(node)
	{
		if(GET_SIZE(node->size)>= size)
		{
			uint32	flg = 1;
			search_fn(node,&flg,size);
			chunk = node;
			if(flg == 0)
			{
				break;
			}
		}
		n = node;
		node = node->right;
		if(node == 0)
		{
			node = n->left;
		}
		if(node == 0)
		{
			node = n->parent;
			if(node)
			{
				node = node->left;
			}
			if(node == n)
			{
				node = 0;
			}
		}
	}
	return	chunk;
}

uint32	high_bytes(uint32 size)
{
	uint32 quo = size/N_BYTES;
	uint32 rem = size - (quo*N_BYTES);
	if(rem == 0)
	{
		return	size;
	}
	else
	{
		return	(size) + (N_BYTES-rem);
	}
}

uint32	low_bytes(uint32 size)
{
	uint32 quo = size/N_BYTES;
	uint32 rem = size - (quo*N_BYTES);
	if(rem == 0)
	{
		return	size;
	}
	else
	{
		return	(quo*N_BYTES);
	}
}

uint32	_guard_bytes()
{
	uint32 g_bytes = GUARD_BYTES;
	if(g_bytes < 0)
	{
		g_bytes = 0;
	}
	g_bytes	=	high_bytes(g_bytes);
	return	g_bytes;
}

uint32	total_size()
{
	uint32 guard_bytes = _guard_bytes();
	uint32	overhead = sizeof(MemoryChunk) + (guard_bytes*2) + sizeof(void*);
	return	overhead;  
}

boolean	is_chunk_size_sufficient(uint32 size)
{
	uint32 min_req_size = total_size();
	min_req_size += N_BYTES;
	if(min_req_size > size)
	{
		return	FALSE;
	}
	else
	{
		return	TRUE;
	}
}

MemoryChunk*	initialize_memory_chunk(void *ptr,uint32 size)
{
	MemoryChunk	*chunk;

	if(ptr == 0 || size == 0)
	{
		return	0;
	}
	if(!is_chunk_size_sufficient(size))
	{
		return	0;
	}
	chunk = (MemoryChunk*)ptr;
	chunk->flag = FREE_CHUNK;
	chunk->avl_flg = 0;
	chunk->size = size - total_size();
	*((uint32*)&(((uint8*)ptr)[size-sizeof(void*)]))	=	(uint32)ptr;
	return	chunk;
}

void insert_footer(MemoryChunk	*chunk)
{
	*((uint32*)&(((uint8*)chunk)[chunk->size + total_size() - sizeof(void*)]))	=	(uint32)chunk;
}

boolean	divide_chunk(MemoryChunk	*chunk,uint32	desired_size,MemoryChunk	**left,MemoryChunk	**right)
{
	void	*ptr;
	uint32	ognl_size;
	uint32 variable;
	if(chunk == 0 || chunk->size < desired_size)
	{
		return	FALSE;
	}
	variable = desired_size;
	desired_size = high_bytes(desired_size);
	if(desired_size-variable>0)
	{
		count = desired_size-variable;
	}
	if(!is_chunk_size_sufficient(chunk->size - desired_size))
	{
		*left = chunk;
		*right = 0;
		return	TRUE;
	}
	ognl_size = chunk->size;
	chunk->size = desired_size;
	insert_footer(chunk);
	*left = chunk;
	ptr = (void*)(&(((uint8*)chunk)[chunk->size + total_size()]));
	initialize_memory_chunk(ptr,ognl_size - desired_size);
	*right = (MemoryChunk*)ptr;
	(*right)->left = (*left)->left;
	(*right)->right = (*left)->right;
	(*right)->parent = (*left)->parent;
	insert_footer(*right);
	return	TRUE;
}

void*	get_requested_memory(MemoryChunk	*chunk)
{
	uint8*	ptr = (uint8*)chunk;
	return	(void*)(ptr + sizeof(MemoryChunk) + _guard_bytes());
	
}

uint32	getRequiredPoolSize(uint32	size)
{
	size = high_bytes(size);
	return	(sizeof(MemoryPool) + size);
}

MemoryPool*	create_memory_pool(void	*ptr,uint32	size)
{
	MemoryPool	*pool;
	uint8	*chunk_start_ptr;
	MemoryChunk	*chunk;	
	if(ptr == 0 || size == 0)
	{
		return	0;
	}
	if(size < sizeof(MemoryPool))
	{
		return	0;
	}
	if(!is_chunk_size_sufficient(size - sizeof(MemoryPool)))
	{
		return	0;
	}
	pool = (MemoryPool*)ptr;
	pool->mem_pool = ptr;
	pool->mem_pool_size = size;
	initialize_binary_tree(&pool->freeChunkTree);
	initialize_binary_tree(&pool->allocatedChunkTree);
	chunk_start_ptr =	(uint8*)ptr;
	chunk_start_ptr += sizeof(MemoryPool);
	chunk	=	initialize_memory_chunk((void*)chunk_start_ptr,size-sizeof(MemoryPool));
	pool->largestChunk	=	chunk;
	insert_chunk(&pool->freeChunkTree,chunk);
	return	pool;
}

void	chunk_availability(MemoryChunk	*chunk,uint32 *flg,uint32 desired_size)
{
	if(desired_size*2 <= chunk->size)
	{
		*flg = 0;
	}
	else
	{
		*flg = 1;
	}
}

MemoryChunk*	find_adequate_chunk_closet(MemoryPool*	pool,uint32 size)
{
	MemoryChunk	*chunk = get_closet_chunk(&pool->freeChunkTree,size,chunk_availability);
	return	chunk;
}

void*	allocate_best_fit(MemoryPool	*pool,uint32 size)
{
	MemoryChunk*	chunk;
	MemoryChunk	*leftChunk,*rightChunk;
	if(pool == 0 || size == 0)
	{
		return	0;
	}
	chunk = find_adequate_chunk_closet(pool,size);
	if(chunk!=NULL)
	{
		printf("Chunk : %d\n",chunk->size);
		remove_chunk(&pool->freeChunkTree,chunk);
		leftChunk = 0;
		rightChunk = 0;
		if(divide_chunk(chunk,size,&leftChunk,&rightChunk))
		{
			leftChunk->flag = ALLOCATED_CHUNK;
			insert_chunk(&pool->allocatedChunkTree,leftChunk);
			if(rightChunk)
			{
				rightChunk->flag = FREE_CHUNK;
				insert_chunk(&pool->freeChunkTree,rightChunk);
			}
			return	get_requested_memory(leftChunk);
		}
		else
		{
			return	0;
		}
	}
	else
			return 0;
}

MemoryChunk*	find_adequate_chunk_bigger(MemoryPool*	pool,uint32 size)
{
	MemoryChunk	*chunk = get_bigger_chunk(&pool->freeChunkTree,size,chunk_availability);
	return	chunk;
}

void*	allocate_worst_fit(MemoryPool	*pool,uint32 size)
{
	MemoryChunk*	chunk;
	MemoryChunk	*leftChunk,*rightChunk;
	if(pool == 0 || size == 0)
	{
		return	0;
	}
	chunk = find_adequate_chunk_bigger(pool,size);
	printf("Chunk : %d\n",chunk->size);
	if(chunk!=0)
	{
		remove_chunk(&pool->freeChunkTree,chunk);
		leftChunk = 0;
		rightChunk = 0;
		if(divide_chunk(chunk,size,&leftChunk,&rightChunk))
		{
			leftChunk->flag = ALLOCATED_CHUNK;
			insert_chunk(&pool->allocatedChunkTree,leftChunk);
			if(rightChunk)
			{
				rightChunk->flag = FREE_CHUNK;
				insert_chunk(&pool->freeChunkTree,rightChunk);
			}
			return	get_requested_memory(leftChunk);
		}
		else
		{
			return	0;
		}
	}
	else
		return 0;
}

void deallocate(MemoryPool	*pool,void	*ptr)
{
	MemoryChunk *chunk = (MemoryChunk*)((uint8*)ptr - sizeof(MemoryChunk) - _guard_bytes());
	remove_chunk(&pool->allocatedChunkTree,chunk);
	chunk = coalesce(pool,chunk);
	chunk->flag = FREE_CHUNK;
	insert_chunk(&pool->freeChunkTree,chunk);
	printf("Free : %d\n", chunk->size);	
}

MemoryChunk* coalesce(MemoryPool* pool,MemoryChunk *chunk)
{
	boolean	loop;
	MemoryChunk *adjacent_chunk;
	uint8	*ptr;
	if(chunk == 0 || pool == 0)
	{		
		return 0;
	}
	for(loop = TRUE;loop==TRUE;)
	{
		if((void*)chunk <= (void*)pool->mem_pool)
		{
			loop = FALSE;
			continue;
		}
			adjacent_chunk = (MemoryChunk*)((uint32*)&(((uint8*)chunk)[chunk->size + total_size()]));
			if(adjacent_chunk->flag == ALLOCATED_CHUNK)
			{
				loop = FALSE;
				continue;
			}
			else
			{
				remove_chunk(&pool->freeChunkTree,adjacent_chunk);
				//printf("Size : %d\n", adjacent_chunk->size + chunk->size + total_size());
				initialize_memory_chunk(chunk,adjacent_chunk->size + chunk->size + total_size()*2);
				return chunk;
				break;
			}
	}
			
			for(loop = TRUE;loop==TRUE;)
			{
			if((uint32)((uint8*)chunk + total_size() + chunk->size) >= (uint32)((uint8*)pool->mem_pool + pool->mem_pool_size))
			{
				loop = FALSE;
				continue;
			}
			ptr = (uint8*)chunk;
			ptr += chunk->size;
			ptr += total_size();
			adjacent_chunk = (MemoryChunk*)ptr;
			if(adjacent_chunk->flag == ALLOCATED_CHUNK)
			{
				loop = FALSE;
				continue;
			}
			if(adjacent_chunk->size==0)
			{
				return 0;
			}
			remove_chunk(&pool->freeChunkTree,adjacent_chunk);
			initialize_memory_chunk(chunk,adjacent_chunk->size + chunk->size + (total_size()*2));
		}
		return chunk;
	}
