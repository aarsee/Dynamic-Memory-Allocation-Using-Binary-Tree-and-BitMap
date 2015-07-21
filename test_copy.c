#include	"MemoryAllocator.h"
#include <stdio.h>

int	main()
{
	uint32 length=10000000;
	char *buffer = (char*)malloc(length+1); 
	MemoryPool	*pool;
	pool = create_memory_pool(buffer,length);	
	void *ptr1,*ptr2,*ptr3,*ptr4;

	ptr1 = allocate_best_fit(pool,4000000);
	if(ptr1==0)
	{
		printf("Allocation 1 cannot be done\n");
	}
	else
	{
		printf("Allocation 1 done\n");
	}

	ptr2 = allocate_best_fit(pool,3000000);
	if(ptr2==0)
	{
		printf("Allocation 2 cannot be done\n");
	}
	else
	{
		printf("Allocation 2 done\n");
	}

ptr3 = allocate_best_fit(pool,6000000);
	if(ptr3==0)
	{
		printf("Allocation 3 cannot be done\n");
	}
	else
	{
		printf("Allocation 3 done\n");
	}

	if(ptr3!=0)
		deallocate(pool,ptr3);
	else
		printf("Block 3 is not allocated\n");
	if(ptr2!=0)
		deallocate(pool,ptr2);
	else
		printf("Block 2 is not allocated\n");

	if(ptr1!=0)
		deallocate(pool,ptr1);
	else
		printf("Block 1 is not allocated\n");
}
