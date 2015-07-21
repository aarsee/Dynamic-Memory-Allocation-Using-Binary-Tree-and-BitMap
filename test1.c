#include	"MemoryAllocator.h"
#include <stdio.h>
int	main()
{
	char	mem[1000];
	MemoryPool	*pool;
	pool = create_memory_pool(mem,1000);	
	void *ptr1,*ptr2,*ptr3,*ptr4;

	ptr1 = allocate_worst_fit(pool,100);
	ptr2 = allocate_worst_fit(pool,100);
	ptr3 = allocate_worst_fit(pool,50);
	deallocate(pool,ptr2);
	ptr4 = allocate_worst_fit(pool,10);
	return	0;
}

