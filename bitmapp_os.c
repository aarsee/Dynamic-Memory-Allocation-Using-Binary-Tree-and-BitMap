#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>

/* Basic constants and macros: */
#define WSIZE      4 /* Word and header/footer size (bytes) */
#define MEMORYSIZE  500000000  /* Extend heap by this amount (bytes) */


/* Global variables: */

char bitmap[MEMORYSIZE/WSIZE];

int
mm_init(void) 
{
	/* Create the initial empty heap. */
	int i = 0;
	for(i = 0; i < MEMORYSIZE/WSIZE; i++){
		bitmap[i] = '0';
	}	

	return (0);
}

int
allocate(int size) 
{
	
	/* Ignore spurious requests. */
	if (size == 0){
		printf("Invalid Request\n");
		return -1;
	}
	//printf("hi im in\n");
	if(size%WSIZE == 0)
		size = size/WSIZE;
	else
		size = size/WSIZE + 1;
	//printf("%d\n", size);
	int i = 0;
	int pos = -1;
	bool flag = false;
	for(i = 0; i<MEMORYSIZE/WSIZE; ){
		if(!flag){
		if(bitmap[i] == '0'){
			//printf("im in 25 loop == i == > %d\n", i);
			int j = i;
			for(j = i; j< i + size; j++){
				if(bitmap[j] != '0'){
					break;
				}
			}
			if( j == i + size){
				//printf("im in the final\n");
				flag = true;
				pos = i;
				break;
			}
			else
				i = i + j;	
		}
		else
			i++;

	}
	}
	if(pos != -1){
		int x;
		for(x = pos; x<i + size; x++){
			bitmap[x] = '1';
		}
	printf("Allocation done\n");
	}
	else
		 printf("This request cannot be accomodated\n");
	return pos;
}

int deallocate(int size, int pos)
{
	int blah = 0;
	//printf("pos  == %d\n",pos);
	//printf("size == %d\n",size);
	/* Ignore spurious requests. */
	if (pos < 0 || pos >= MEMORYSIZE || bitmap[pos] == '0'){
		printf("Invalid Request\n");
		return -1;
	}
    else{
    	if(size % WSIZE == 0)
    		size = size/WSIZE;
    	else
    		size = size/WSIZE + 1;
    	//printf("im in free\n");
    	int i = pos;
    	for(i = pos; i< pos + size; i++){
    		if(bitmap[i] == '0'){
    			//printf("im in wornf loop\n");
    			blah = -1;
    			break;
    		}
    		else 
    			bitmap[i] = '0';
    	}
    	if(blah == -1){
    		printf("Invalid Request\n");
    		return -1;
    	}
    	else{
		printf("Deallocation done\n");	
    		return 0;
	}
    }	
}

int main(){
	clock_t start,end;
	start = clock();
	int initial = mm_init();
	//printf("helooooooo\n");
	//printf("bit map initially = %s\n", bitmap);
	//printf("value of initial = %d\n", initial);
	int a1,a2,a3,a4,a5;
	int d1,d2,d3,d4,d5;
	int bounced_requests = 0;
	a1 = allocate(400000);
	printf("*********\n");
	if(a1 == -1)
		bounced_requests ++;
	//printf("bitmaap after maloc of 100 = %s\n", bitmap);
	//printf("vlue of malloc of 4  = %d\n", mallc1);
	a2 = allocate(600000000);
	printf("*********\n");
	a3 = allocate(300000000);
	printf("*********\n");
	d1 = deallocate(600000000,a3);
	printf("*********\n");
	d2 = deallocate(400000,a1);
	printf("*********\n");
	d3 = deallocate(60000,a2);
	printf("*********\n");
	a4 = allocate(48000);
	printf("*********\n");
	if(a2 == -1)
		bounced_requests ++;
		if(a3 == -1)
		bounced_requests ++;
		if(a4 == -1)
		bounced_requests ++;
	//printf("vlue of malloc of 9 =  %d\n", mallc2);
	//printf("bitmap afteer mallc of 200 = %s\n", bitmap);
	//printf("done\n");
	//d1 = deallocate(2000,a1);
	//printf("d1 => %d\n", d1);
	//d2 = deallocate(400000,a2);
	//printf("%d\n",d2 );
	//printf("bit map after freeing size 4 at pos 0 => %s\n", bitmap);
	//mallc3 = mm_malloc(50000);
	//mallc3 = mm_malloc(50000);
	//mallc3 = mm_malloc(50000);
	//mallc3 = mm_malloc(50000);
	//if(mallc3 == -1)
		//fragment_count ++;
	//printf("mallc 3 of sie 2 => %d\n", mallc3);
	//printf("%s\n", bitmap);
	//frr = mm_free(9,mallc2);
	
	//printf("%d\n", frr);
	//printf("%s\n", bitmap);
	
	printf("%d\n", bounced_requests);
	end = clock();
	float t = (float)((end-start))/CLOCKS_PER_SEC;
	printf("Time : %f", t);
}
