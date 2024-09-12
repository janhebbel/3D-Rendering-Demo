// Code inspired by Chris Wellons: https://nullprogram.com/blog/2023/09/27/

typedef struct Arena {
	byte *buf;
	size_t buf_len;
	size_t offset;
	size_t prev_offset;
} Arena;

Arena arena_create(void *backing_buf, size_t backing_buf_len) {
	assert(backing_buf != NULL && backing_buf_len != 0);
	
	Arena a;
	a.buf = (byte *)backing_buf;
	a.buf_len = backing_buf_len;
	a.offset = 0;
	a.prev_offset = 0;
	
	return a;
}

void *arena_alloc_align(Arena *a, size_t size, size_t align) {
	uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->offset;
	uintptr_t offset = align_forward(curr_ptr, align);
	offset -= (uintptr_t)a->buf;

	if (offset + size <= a->buf_len) {
		void *ptr = a->buf + offset;
		a->prev_offset = offset;
		a->offset = offset + size;
		memset(ptr, 0, size);
		return ptr;
	}

	// Arena out of memory.
	assert(0 && "Bik Problem!");
	return NULL;
}

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2*sizeof(void *)) 
#endif

void *arena_alloc(Arena *a, size_t size) {
	return arena_alloc_align(a, size, DEFAULT_ALIGNMENT);
}

void *arena_resize_align(Arena *a, void *old_buf, size_t old_size, size_t new_size, size_t align) {
	byte *old_mem = (byte *)old_buf;

	assert(is_power_of_two(align));

	if (old_mem == NULL || old_size == 0)
		return arena_alloc_align(a, new_size, align);
	else if (a->buf <= old_mem && old_mem < a->buf + a->buf_len) {
		if (a->buf + a->prev_offset == old_mem) {
			a->offset = a->prev_offset + new_size;
			if (new_size > old_size)
				memset(a->buf + a->offset, 0, new_size - old_size);
			return old_buf;
		} else {
			void *new_memory = arena_alloc_align(a, new_size, align);
			size_t copy_size = old_size < new_size ? old_size : new_size;
			memmove(new_memory, old_buf, copy_size);
			return new_memory;
		}
	} else {
		assert(0 && "The memory in this arena is out of bounds of the buffer.");
		return NULL;
	}
}

void *arena_resize(Arena *a, void *old_buf, size_t old_size, size_t new_size) {
	return arena_resize_align(a, old_buf, old_size, new_size, DEFAULT_ALIGNMENT);
}

void arena_free_all(Arena *a) {
	a->offset = 0;
	a->prev_offset = 0;
}
