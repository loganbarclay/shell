
/**
 * Implementation of a memory allocator based on the Buddy System.
 * See Knuth Art of Computer Programming, vol. 1, page 442.
 * Each available block starts with a header that consists
 * of a tag (free/reserved), kval (size of block 2^kval), next
 * and previous pointers. Each reserved block has the tag and kval
 * field only. All allocations are done in roundUps of two. All requests
 * are rounded up to the next roundUp of two.
 *
 * @author  Logan Barclay
 *
 */

#include "buddy.h"
int initialized = FALSE;

/* the header for an available block */
struct block_header {
	short tag;
	short kval;
	struct block_header *next;
	struct block_header *prev;
};

typedef struct block_header Header;

const int RESERVED = 0;
const int FREE = 1;
const int UNUSED = -1;


/* supports memory upto 2^(MAX_KVAL-1) (or 32 GB) in size */
#define  MAX_KVAL  35  //4GB
#define  MIN_KVAL  10  //1KB
#define  KVAL      29  //512MB
#define  NO_MEM    NULL

static size_t roundUp(size_t);
static int getKval(size_t);
static int getBlock(int);
static void* getBuddy(int, void*);

/* deafult memory allocation is 512MB */
const size_t DEFAULT_MAX_MEM_SIZE = 512 * 1024 * 1024;


/* A static structure stores the table of pointers to the lists in the buddy system.  */
struct pool {
	void *start; // pointer to the start of the memory pool
	int lgsize;  // log2 of size
	size_t size; // size of the pool, same as 2 ^ lgsize
	/* the table of pointers to the buddy system lists */
	struct block_header avail[MAX_KVAL];
} pool;



void buddy_init(size_t size) {

	Header *hdr;
	int i;
	//unsigned long block_size;

	if (!initialized) {

		size = roundUp(size);
		pool.lgsize = getKval(size);

		if (pool.lgsize > MAX_KVAL ) {
			size = 1UL << MAX_KVAL;
			pool.lgsize = MAX_KVAL;
		}

		if (pool.lgsize < MIN_KVAL) {
			size = 1UL << KVAL;
			pool.lgsize = KVAL;
		}

		pool.start = sbrk(size);

		if (pool.start < 0) {
			perror("allocation error");
			exit(1);
		}

		for (i = 0; i < pool.lgsize; i++)
		{
			pool.avail[i].next = &pool.avail[i];
			pool.avail[i].prev = &pool.avail[i];
			pool.avail[i].tag = RESERVED;
			pool.avail[i].kval = i;
		}

		pool.avail[i].next = pool.start;
		pool.avail[i].prev = pool.start;
		pool.avail[i].tag = FREE;
		pool.avail[i].kval = pool.lgsize;

		hdr = pool.avail[i].next;
		hdr->next = &pool.avail[i];
		hdr->prev = &pool.avail[i];
		hdr->tag = FREE;
		hdr->kval = i;

	}

	initialized = TRUE;
	//printf("value of m:  %d\n", m);
}


void *buddy_malloc(size_t size) {

	int i = 0;
	int kval = 0;
	if (size == 0) {
		return NULL;
	}
	if (!initialized) {
		buddy_init(1 << KVAL);
	}

	Header *hdr_one = NULL;
	Header *hdr_two = NULL;

	size_t block_size = size + sizeof(Header);
	//printf("malloc/header %ld\n", block_size);
	block_size = roundUp(block_size);
	//printf("bytes %ld\n", block_size);
	kval = getKval(block_size);
	//printf("value of k:  %d\n", k);
	if (kval > pool.lgsize) {
		errno = ENOMEM;
		return (void*) NO_MEM;
	}

	i = getBlock(kval);
	//printf("value of j:  %d\n", j);

	if (i != UNUSED) {
		hdr_one = pool.avail[i].next;

		pool.avail[i].next = hdr_one->next;
		hdr_one->next->prev = &pool.avail[i];
		hdr_one->tag = RESERVED;
		hdr_one->kval = kval;

		while (i != kval) {

			i--;
			long htAddress = (long)hdr_one + (1 << i);

			hdr_two = (Header*) htAddress;

			hdr_two->next = &pool.avail[i];
			hdr_two->prev = &pool.avail[i];
			hdr_two->tag = FREE;
			hdr_two->kval = i;
			pool.avail[i].next = hdr_two;
			pool.avail[i].prev = hdr_two;
			pool.avail[i].tag = FREE;
		}

	} else {
		errno = ENOMEM;
		return (void*) NO_MEM;
	}

	hdr_one++;
	//printf("Got here");
	return (void *) hdr_one;
}


void *buddy_calloc(size_t nmemb, size_t size)
{
	return NULL;
}

void *buddy_realloc(void *ptr, size_t size)
{
	return NULL;
}


void buddy_free(void *ptr) {

	Header *hdr_one = NULL;
	Header *hdr_two = NULL;
	int kval;

	if (ptr == NULL) {
		return;
	}

	hdr_one = (Header*)(ptr) - 1;
	kval = hdr_one->kval;

	void* buddyAddress = getBuddy(kval, (void*) hdr_one);

	hdr_two = (Header *) (buddyAddress);

	// combine with buddy
	// while k not max || hdr_two tag is not reserved || check hdr_two is free but not the right kval
	while ( !(( kval == pool.lgsize ) || ( hdr_two->tag == RESERVED ) 
					|| (( hdr_two->tag == FREE) && ( hdr_two->kval != kval)))) {
		
		hdr_two->prev->next = hdr_two->next;
		hdr_two->next->prev = hdr_two->prev;

		kval++;

		if (hdr_two < hdr_one) {
			hdr_one = hdr_two;
		}
		hdr_two = (Header *)(getBuddy(kval, (void*) hdr_one));
	}

	hdr_one->tag = 1;
	hdr_one->next = pool.avail[kval].next;
	pool.avail[kval].next->prev = hdr_one;
	hdr_one->kval = kval;
	hdr_one->prev = &pool.avail[kval];
	pool.avail[kval].next = hdr_one;
	
}


void printBuddyLists() {
	int i = 0;
	int available = 0;

	while (i <= pool.lgsize) {
		printf("List %d head = %p --> [tag=%d,kval=%d,addr=%p] ", i , &pool.avail[i], pool.avail[i].tag, pool.avail[i].kval, pool.avail[i].next);
		Header* hdr = &pool.avail[i];
		while (hdr->next != &pool.avail[i]) {
			printf("-->%p", hdr);
			hdr = hdr->next;
		}
		printf("--> <null>\n");
		if(pool.avail[i].tag == FREE){
			available++;
		}
		i++;
	}
	printf("\nNumber of available blocks = %d\n", available);
	available = 0;
}

static size_t roundUp(size_t size) {
	
	size_t block_size = 1;
	
	while (block_size < size) {
		block_size = block_size * 2;
	}
	return block_size;
}

static int getKval(size_t block_size) {
	
	int kval = 0;
	block_size--;

	while (block_size > 0) {
		block_size >>= 1;
		kval++;
	}
	return kval;
}

static int getBlock(int kval) {
	int block;

	for (block = kval; block <= pool.lgsize; block++)
	{
		if (pool.avail[block].next != &pool.avail[block])
		{
			return block;
		}
	}
	return -1;
}

static void* getBuddy(int kval, void* hdr) {
	long j = (long) kval;
	long y = (long) hdr;

	return (void*) (((1 << j) ^ (y - (long)pool.start)) + (long)pool.start);

}
