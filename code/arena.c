// Code inspired by Chris Wellons: https://nullprogram.com/blog/2023/09/27/

typedef struct Arena {
	void *memory;
	ptrdiff_t offset;
	ptrdiff_t capacity;
} Arena;

Arena arena_create(ptrdiff_t capacity) {
	Arena a;
	a.memory = malloc(capacity);
	a.offset = 0;
	a.capacity = a.memory ? capacity : 0;
	return a;
}

void *arena_alloc(Arena *a, ptrdiff_t size) {
	ptrdiff_t available = ((byte *)a->memory + a->capacity) - ((byte *)a->memory + a->offset);
	if (size < 0 || size > available) {
		assert(!"Uh oh. You need to buy more RAM!");
		exit(-1);
	}
	void *alloc = (byte *)a->memory + a->offset;
	a->offset += size;
	return memset(alloc, 0, size);
}

void arena_destroy(Arena *a) {
	free(a->memory);
	a->memory = NULL;
	a->offset = 0;
	a->capacity = 0;
}
