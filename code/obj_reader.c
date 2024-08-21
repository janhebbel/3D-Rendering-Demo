enum token_type {
	// Punctuation is hidden part of the struct as ascii
	ITEM_NONE = 256,
	ITEM_v,
	ITEM_vn,
	ITEM_vt,
	ITEM_vp,
	ITEM_f,
	ITEM_o,
	ITEM_NUMBER,
	ITEM_IDENTIFIER
};

struct token {
	enum token_type type;
	char *value;
};

struct token_list {
	struct token *array;
	char *values;
};


enum lexer_state {
	STATE_WHITESPACE,
	STATE_IN_WORD,
	STATE_WORD_END
};

struct scene_result {
	bool success;
	struct scene scene;
};

char *append_string(char **string_array, char *source, int string_start, int string_length) {
	for(int i = 0; i < string_length; i++) {
		darr_append(*string_array, source[string_start + i]);
	}
	darr_append(*string_array, '\0');
	char *first = *string_array + darr_length(*string_array) - string_length - 1;
	return(first);
}

struct token_list lex(char *file, int file_size) {
	struct token_list tokens = {0};

	int current_index = 0;
	char current_char;

	enum lexer_state state = STATE_WHITESPACE;

	int word_start = 0;

	// TODO: test if progress_idnex >= file_size && state == STATE_WHITESPACE is correct
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
			bool word_end_condition = is_whitespace(current_char) || current_char == '/' || 
				file[word_start] == '/' || current_index == file_size;
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
				darr_append(tokens.array, token);
				state = STATE_WHITESPACE;
			}
			else if(strings_are_equal_1l(word_length, &file[word_start], "vt")) {
				token.type = ITEM_vt;
				darr_append(tokens.array, token);
				state = STATE_WHITESPACE;
			}
			else if(strings_are_equal_1l(word_length, &file[word_start], "vn")) {
				token.type = ITEM_vn;
				darr_append(tokens.array, token);
				state = STATE_WHITESPACE;
			}
			else if(strings_are_equal_1l(word_length, &file[word_start], "vp")) {
				token.type = ITEM_vp;
				darr_append(tokens.array, token);
				state = STATE_WHITESPACE;
			}
			else if(strings_are_equal_1l(word_length, &file[word_start], "f")) {
				token.type = ITEM_f;
				darr_append(tokens.array, token);
				state = STATE_WHITESPACE;
			}
			else if(strings_are_equal_1l(word_length, &file[word_start], "o")) {
				token.type = ITEM_o;
				darr_append(tokens.array, token);
				state = STATE_WHITESPACE;
			}
			else { 
				// => TODO: maybe implement a lexer struct which has a function pointer - lookahead() and consume() or something
				if(file[word_start] == '-' && word_length > 1 && is_numeric(file[word_start+1]) || 
					is_numeric(file[word_start])) {
					token.type = ITEM_NUMBER;
					token.value = append_string(&tokens.values, file, word_start, word_length);
				}
				else if(is_alphabetic(file[word_start])) {
					token.type = ITEM_IDENTIFIER;
					token.value = append_string(&tokens.values, file, word_start, word_length);
				}
				else if(file[word_start] == '/') {
					token.type = '/';
					token.value = append_string(&tokens.values, file, word_start, word_length);    
				}
				else {
					assert(!"Unrecognized symbol.");
					return(tokens);
				}

				darr_append(tokens.array, token);
				state = STATE_WHITESPACE;
			}
			word_length = 0;

			if(current_index >= file_size) 
				goto end_loop;

			break;

		default:
			assert(!"Entered unknown state!");
			return(tokens);
		}
	}

end_loop:
	return(tokens);
}

struct scene parse(struct token_list tokens) {
	struct scene scene = {0};
	return(scene);
}

struct scene obj_parse(byte *obj_file_data, int file_size) {
	struct token_list tokens = lex((char *)obj_file_data, file_size);
	return(parse(tokens));
}
