//
// Useful Macros

#define ARRAYCOUNT(x) sizeof(x) / sizeof(x[0])

#define ZEROSTRUCT(x) memset(&(x), 0, sizeof(x))
#define STUB(type) ((type){0})

#define MINIMUM(x, y) ((x) < (y)) ? (x) : (y)
#define MAXIMUM(x, y) ((x) > (y)) ? (x) : (y)
#define SIGN(x) ((x > 0) - (x < 0))
#define ABS(value) ((value < 0) ? -(value) : (value))
#define CLAMP(x, max, min) MAXIMUM(MINIMUM(x, max), min)

#define KILOBYTES(x) ((x) << 10)
#define MEGABYTES(x) ((x) << 20)
#define GIGABYTES(x) ((x) << 30)

#include <string.h>

//
// Useful types

typedef struct String_View {
	char *start;
	int length;
} String_View;

//
// Useful Functions

inline int is_end_of_line(char c) {
	return((c == '\n') || (c == '\r'));
}

inline int is_spacing(char c) {
	return((c == ' ') || (c == '\t') || (c == '\v') || (c == '\f'));
}

inline int is_whitespace(char c) {
	return(is_spacing(c) || is_end_of_line(c));
}

inline int is_alphabetic(char c) {
	return(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}

inline int is_numeric(char c) {
	return(c >= '0' && c <= '9');
}

inline int is_alphanumeric(char c) {
	return(is_alphabetic(c) || is_numeric(c));
}

inline int strings_are_equal(char *a, char *b) {
	int result = (a == b);
	
	if(a && b) {
		while(*a && *b && (*a == *b)) {
			++a;
			++b;
		}
		
		result = ((*a == 0) && (*b == 0));
	}
	
	return(result);
}

inline int strings_are_equal_1l(int a_length, char *a, char *b) {
	int result = false;
	
	if(b) {
		char *at = b;
		for(uintptr_t i = 0; i < a_length; ++i, ++at) {
			if(b[i] == '\0' || (a[i] != b[i])) {
				return(false);
			}
		}
		
		result = (*at == 0);
	}
	else {
		result = (a_length == 0);
	}
	
	return(result);
}

inline int strings_are_equal_2l(int a_length, char *a, int b_length, char *b) {
	int result = (a_length == b_length);

	if(result) {
		result = true;
		for(int i = 0; i < a_length; ++i) {
			if(a[i] != b[i]) {
				result = false;
				break;
			}
		}
	}
	return(result);
}

inline int round_float(float val) {
	// if val is positive 1 - 0 = +1,
	// if val is negative 0 - 1 = -1.
	int   sign = (val > 0) - (val < 0);
	float half = 0.5;

	return((int)(val + half * sign));
}

inline float string_to_float(char *str, int len) {
	char null_terminated_str[16] = {0};
	memcpy(null_terminated_str, str, len);
	float f = strtof(null_terminated_str, NULL);
	return(f);
}

inline int string_to_int(char *str, int len) {
	char null_terminated_str[11] = {0};
	memcpy(null_terminated_str, str, len);
	int i = (int)strtol(null_terminated_str, NULL, 10);
	return i;
}

inline int parse_one_word(char *str, int len, int offset, int *word_len_out) {
	int start_idx = 0, word_len = 0;
	while(offset + start_idx < len && is_whitespace(str[offset + start_idx])) ++start_idx;
	while(offset + start_idx + word_len < len && !is_whitespace(str[offset + start_idx + word_len])) ++word_len;
	if(word_len_out) *word_len_out = word_len;
	return(start_idx);
}

byte *read_file(char *filename, int *filesize) {
	if (!filename) {
		return NULL;
	}
	
	FILE *file = fopen(filename, "r");
	fseek(file, 0, SEEK_END);
	int size = ftell(file) + 1;
	rewind(file);
	
	void *file_data = malloc(size);
	if (!file_data) {
		assert(!"Malloc failed.");
		exit(-1);
	}
	
	size = (int)fread(file_data, 1, size, file);
	if (ferror(file)) {
		assert(!"Error.");
		return NULL;
	}
	
	if (filesize) *filesize = size;
	
	return file_data;
}

inline void *smalloc(size_t size) {
	void *m = malloc(size);
	if (!m) {
		assert(!"Failed malloc!");
		exit(-1);
	}
	return memset(m, 0, size);
}

inline void *srealloc(void *ptr, size_t size) {
	void *n = realloc(ptr, size);
	if (!n) {
		assert(!"Failed realloc!");
		exit(-1);
	}
	return memset(n, 0, size);
}

// Credit: Tsoding
// this macro assumes that things is a struct consisting of the following fields:
// * items
// * count
// * cap
#define da_append(things, thing)\
	do {\
		if (things.count >= things.cap) {\
			if (things.cap == 0) things.cap = 256;\
			else things.cap *= 2;\
			things.items = srealloc(things.items, things.cap * sizeof(things.items[0]));\
		}\
		things.items[things.count++] = thing;\
	} while(0)
		
