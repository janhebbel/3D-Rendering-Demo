enum token_type {
	ITEM_v, // this has to be the first token type
	ITEM_vn,
	ITEM_vt,
	ITEM_vp,
	ITEM_f,
	ITEM_o, // this has to be the last token type
	ITEM_NUMBER,
	ITEM_IDENTIFIER,
	ITEM_SLASH
};

#define TOKEN_IS_KEYWORD(token) ((token).type >= ITEM_v && (token).type <= ITEM_o)

struct token {
	enum token_type type;
	struct string_view value;
};

struct token_list {
	struct token *array;
	int length;
	int capacity;
};

// Allocates initial_size memory for the token_list array. Returns false if it fails.
struct token_list token_list_init(const int initial_size) {
	struct token_list token_list;
	token_list.length = 0;
	token_list.array = malloc(sizeof(struct token) * initial_size);
	token_list.capacity = (token_list.array == NULL) ? 0 : initial_size;
	return token_list;
}

// Returns false if list needed to grow but couldnt, else returns true.
bool token_list_maybe_grow(struct token_list *token_list) {
	if(token_list->length == token_list->capacity) {
		token_list->capacity *= 2;
		void *old_alloc = token_list->array;
		token_list->array = realloc(token_list->array, token_list->capacity);
		if(token_list->array == NULL) {
			free(old_alloc);
			token_list->array = NULL;
			token_list->length = 0;
			token_list->capacity = 0;
			return false;
		}
	}
	
	return true;
}


// Adds a token to the list. Grows the list if necessary. If growing fails, false is returned, else true.
bool token_list_add(struct token_list *token_list, const struct token *token) {
	// token list fails to grow?
	if(!token_list_maybe_grow(token_list)) {
		return false;
	}
	token_list->array[token_list->length++] = *token;
	return true;
}

struct token token_list_at(struct token_list *token_list, int index) {
	if(index >= token_list->length) {
		return STUB(struct token);
	}
	return token_list->array[index];
}

enum lexer_state {
	STATE_WHITESPACE,
	STATE_IN_WORD,
	STATE_WORD_END
};

struct scene_result {
	bool success;
	struct scene scene;
};

struct token_list lexer_run(char *file, int file_size) {
	struct token_list token_list = token_list_init(file_size / 2);
	
	if(token_list.capacity == 0) {
		assert(!"Something went wrong here.");
		return token_list;
	}

	int current_index = 0;
	char current_char;

	enum lexer_state state = STATE_WHITESPACE;

	int word_start = 0;

	// TODO: test if progress_index >= file_size && state == STATE_WHITESPACE is correct
	while(1) { 
		current_char = file[current_index < file_size ? current_index : file_size - 1];

		switch(state) {
		case STATE_WHITESPACE:
			if(is_whitespace(current_char)) {
				current_index++;
			}
			else {
				state = STATE_IN_WORD,
				word_start = current_index;
				current_index++;
			}
			break;

		case STATE_IN_WORD:
			bool word_end_condition = is_whitespace(current_char) || current_char == '/' 
				|| file[word_start] == '/' || current_index == file_size;
			if(word_end_condition) {
				state = STATE_WORD_END;
			}
			else {
				current_index++;
			}
			break;

		case STATE_WORD_END: 
			struct token token = {0};
			int word_length = current_index - word_start;
			if(strings_are_equal_1l(word_length, &file[word_start], "v")) {
				token.type = ITEM_v;
			}
			else if(strings_are_equal_1l(word_length, &file[word_start], "vt")) {
				token.type = ITEM_vt;
			}
			else if(strings_are_equal_1l(word_length, &file[word_start], "vn")) {
				token.type = ITEM_vn;
			}
			else if(strings_are_equal_1l(word_length, &file[word_start], "vp")) {
				token.type = ITEM_vp;
			}
			else if(strings_are_equal_1l(word_length, &file[word_start], "f")) {
				token.type = ITEM_f;
			}
			else if(strings_are_equal_1l(word_length, &file[word_start], "o")) {
				token.type = ITEM_o;
			}
			else { 
				if(file[word_start] == '-' && word_length > 1 && is_numeric(file[word_start+1]) || 
					is_numeric(file[word_start])) {
					token.type = ITEM_NUMBER;
					token.value = (struct string_view){file + word_start, word_length};
				}
				else if(is_alphabetic(file[word_start])) {
					token.type = ITEM_IDENTIFIER;
					token.value = (struct string_view){file + word_start, word_length};
				}
				else if(file[word_start] == '/') {
					token.type = ITEM_SLASH;  
				}
				else {
					assert(!"Unrecognized symbol.");
					return token_list;
				}
			}
			
			token_list_add(&token_list, &token);
			state = STATE_WHITESPACE;
			
			word_length = 0;

			if(current_index >= file_size) 
				goto end_loop;

			break;

		default:
			assert(!"Entered unknown state!");
			return token_list;
		}
	}

end_loop:
	return token_list;
}

bool is_obj_float(char *string, int length) {
	if(length == 0 || string == NULL)
		return false;
	
	char c = string[0];
	if(!(c == '-' || c == '+' || is_numeric(c) || c == '.')) // float has to start with - + 0-9 .
		return false;
	else {
		if((c == '-' || c == '+' || c == '.') && length == 1) // dont allow only + - .
			return false;
	}
	
	if(string[length - 1] == '.')
		return false;
	
	bool dot = false; // only one dot allowed so keep track of it here
	
	for(int i = 1; i < length; i++) {
		char prev_c = c;
		c = string[i];
		
		if(c == '.') {
			dot = true;
			if (prev_c == '+' || prev_c == '-')
				return false;
			continue;
		}
		
		if(dot && c == '.' || !(is_numeric(c))) {
			return false;
		}
	}
	
	return dot;
}

bool is_obj_uint(char *string, int length) {
	if(length == 0 || string == NULL) {
		return false;
	}
	
	for(int i = 0; i < length; ++i) {
		char c = string[i];
		if(!is_numeric(c)) {
			return false;
		}
	}
	
	return true;
}

void test(void) {
	char *tests[] = { "+", ".", "+.", "1", "abc", "1.", ".1", "a.1", "123412341234.1", "1.123412341234", "1.1234e12341234", "-1.1", "+1.1", "1.1", "1,1" };
	for(int i = 0; i < (int)ARRAYCOUNT(tests); ++i) {
		bool test = is_obj_float(tests[i], (int)strlen(tests[i]));
		test;
	}
}

struct scene parse(const struct token_list *token_list) {
	struct scene s = {0};
	
	int vertex_count = 0, index_count = 0, texcoord_count = 0, normal_count = 0;
	
	for(int i = 0; i < token_list->length; i++) {
		struct token current_token = token_list->array[i];
		
		switch(current_token.type) {
		case ITEM_v:
			vertex_count++;
			break;
		case ITEM_vt:
			texcoord_count++;
			break;
		case ITEM_vn:
			normal_count++;
			break;
		case ITEM_f:
			i++;
			while(i < token_list->length && (token_list->array[i].type == ITEM_NUMBER || token_list->array[i].type == ITEM_SLASH)) {
				if(token_list->array[i].type == ITEM_NUMBER && token_list->array[i-1].type == ITEM_NUMBER) {
					index_count++;
				}
				i++;
			}
			i--;
		}
	}
	
	// TODO: replace with arena allocation
	vec4 *vertices = calloc(vertex_count, sizeof(vec4));    // x, y, z, [w]
	vec3 *texcoords = calloc(texcoord_count, sizeof(vec3)); // u, [v, w]
	vec3 *normals = calloc(normal_count, sizeof(vec3));     // x, y, z
	
	if(vertices == NULL || texcoords == NULL || normals == NULL) {	
		s = STUB(struct scene);
		goto cleanup;
	}
	
	int mode = -1;	
	int vidx = 0, tidx = 0, nidx = 0;
	
	for(int i = 0; i < token_list->length; i++) {
		struct token current_token = token_list->array[i];
		
		switch(current_token.type) {
		case ITEM_v:
			mode = ITEM_v;
			break;
		case ITEM_vt:
			mode = ITEM_vt;
			break;
		case ITEM_vn:
			mode = ITEM_vn;
			break;
		case ITEM_NUMBER:
			int initial_i = i;
			switch(mode) {
			case ITEM_v:
				while(i < token_list->length && i < initial_i + 4 && token_list->array[i].type == ITEM_NUMBER) {
					if(!is_obj_float(token_list->array[i].value.start, token_list->array[i].value.length)) {
						s = STUB(struct scene);
						assert(!"Error while parsing! Expected float.");
						goto cleanup;
					}
					vertices[vidx][i - initial_i] = string_to_float(token_list->array[i].value.start, token_list->array[i].value.length);
					i++;
				}
				i--;
				vidx++;
				break;
			case ITEM_vt:
				while(i < token_list->length && i < initial_i + 3 && token_list->array[i].type == ITEM_NUMBER) {
					if(!is_obj_float(token_list->array[i].value.start, token_list->array[i].value.length)) {
						s = STUB(struct scene);
						assert(!"Error while parsing! Expected float.");
						goto cleanup;
					}
					texcoords[tidx][i - initial_i] = string_to_float(token_list->array[i].value.start, token_list->array[i].value.length);
					i++;
				}
				i--;
				tidx++;
				break;
			case ITEM_vn:
				while(i < token_list->length && i < initial_i + 3 && token_list->array[i].type == ITEM_NUMBER) {
					if(!is_obj_float(token_list->array[i].value.start, token_list->array[i].value.length)) {
						s = STUB(struct scene);
						assert(!"Error while parsing! Expected float.");
						goto cleanup;
					}
					normals[nidx][i - initial_i] = string_to_float(token_list->array[i].value.start, token_list->array[i].value.length);
					i++;
				}
				i--;
				nidx++;
				break;
			}
			break;
		default:
			mode = -1;
		}
	}
	
	// TODO: replace with arena allocation
	s.vertex_count = vertex_count;
	s.vertex_array = calloc(s.vertex_count, sizeof(struct textured_vertex));
	
	s.index_count = index_count;
	s.index_array = calloc(s.index_count, sizeof(unsigned int));
	
	if(s.vertex_array == NULL || s.index_array == NULL) {
		s = STUB(struct scene);
		goto cleanup2;
	}
	
	int insert_index_vertex = 0, insert_index_index = 0;
	for(int i = 0; i < token_list->length; i++) {
		struct token current_token = token_list->array[i];
		
		if(current_token.type == ITEM_f) {
			i++;
			struct textured_vertex v = {0};
			int slash_count = 0;
			while(i < token_list->length && (token_list->array[i].type == ITEM_NUMBER || token_list->array[i].type == ITEM_SLASH)) {
				if(token_list->array[i].type == ITEM_NUMBER && token_list->array[i-1].type == ITEM_NUMBER) {
					s.vertex_array[insert_index_vertex++] = v;
					s.index_array[insert_index_index] = insert_index_index;
					insert_index_index++;
					slash_count = 0;
				}
				
				if(token_list->array[i].type == ITEM_NUMBER) {
					if(!is_obj_uint(token_list->array[i].value.start, token_list->array[i].value.length)) {
						s = STUB(struct scene);
						assert(!"Error! Unsigned integer expected.");
						goto cleanup2;
					}
					unsigned int index = (unsigned int)string_to_int(token_list->array[i].value.start, token_list->array[i].value.length);
					index -= 1; // obj indices start counting at 1
					switch(slash_count) {
					case 0:
						memcpy(v.position, &vertices[index], sizeof(v.position));
						s.index_array[insert_index_index++] = index;
						break;
					case 1:
						memcpy(v.uv, &texcoords[index], sizeof(v.uv));
						break;
					case 2:
						memcpy(v.normal, &normals[index], sizeof(v.normal));
						break;
					default:
						s = STUB(struct scene);
						assert(!"Error! Too many indices.");
						goto cleanup2;
					}
				}
				else if(token_list->array[i].type == ITEM_SLASH) {
					slash_count++;
				}
				i++;
			}
			i--;
		}
	}
	
cleanup2:	
	if(s.vertex_array) free(s.vertex_array);
	if(s.index_array) free(s.index_array);
cleanup:
	if(vertices) free(vertices);
	if(texcoords) free(texcoords);
	if(normals) free(normals);
	return s;
}

// TODO: write a lexer struct which will be used for lexing/parsing
//       implement consume(), expect(), peek(), ...
//       also, maybe dont lex all the way, lex only when parser needs it

struct scene obj_parse(byte *obj_file_data, int file_size) {
	struct token_list token_list = lexer_run((char *)obj_file_data, file_size);
	return parse(&token_list);
}
