enum Token_Type {
	ITEM_v, // this has to be the first token type
	ITEM_vn,
	ITEM_vt,
	ITEM_vp,
	ITEM_f,
	ITEM_o, // this has to be the last token type
	ITEM_FLOAT,
	ITEM_INT,
	ITEM_IDENTIFIER,
	ITEM_SLASH
};

#define TOKEN_IS_KEYWORD(token) ((token).type >= ITEM_v && (token).type <= ITEM_o)

typedef struct Token {
	enum Token_Type type;
	union {
		int i;
		float f;
		String_View s;
	} value;
} Token;

typedef struct Tokens {
	Token *items;
	int count;
	int cap;
} Tokens;

Tokens tokens_init(int initial_count) {
	Tokens ts = {0};
	ts.items = smalloc(initial_count * sizeof(*ts.items));
	ts.count = 0;
	ts.cap = initial_count;
	return ts;
}

void tokens_append(Tokens *ts, Token t) {
	if (ts->count >= ts->cap) {
		ts->cap *= 2;
		ts->items = srealloc(ts->items, ts->cap * sizeof(ts->items[0]));
	}
	ts->items[ts->count++] = t;
}

//
// Lexer 

typedef struct Lexer {
	char *file;
	int file_size;
	
	int index;
	int state;
} Lexer;

bool is_obj_int(char *string, int length) {
	assert(length != 0 && string != NULL);
	char c = string[0];
	if (!is_numeric(c) && c != '+' && c != '-')
		return false;
	for(int i = 1; i < length; ++i) {
		c = string[i];
		if(!is_numeric(c)) {
			return false;
		}
	}
	return true;
}

// advances the internal index if s and string are equal
bool lexer_maybe(Lexer *l, String_View s, char *string) {
	if (strings_are_equal_1l(s.length, s.start, string)) {
		l->index += s.length;
		return true;
	}
	return false;
}

bool lexer_maybe_int(Lexer *l, String_View s) {
	if (is_obj_int(s.start, s.length)) {
		l->index += s.length;
		return true;
	}
	return false;
}

// skip any whitespaces
void lexer_advance(Lexer *l) {
	while (l->index < l->file_size && is_whitespace(l->file[l->index]))
		l->index++;
}

// TODO: Bump Allocator
Tokens tokenize(Lexer *l) {
	// Using heuristic that assumes #tokens < file_size to avoid reallocations.
	Tokens tokens = tokens_init(l->file_size);
	
	// int word_start = 0;
	// int word_length = 0;
	
	// bool dot = false;
	
	// scanning line by line
	while (l->index < l->file_size) {
		lexer_advance(l);
		
		if (l->file[l->index] == '#') {
			// comment -> ignore
			while (l->index < l->file_size && !is_end_of_line(l->file[l->index])) l->index++;
			lexer_advance(l);
		}
		
		// scan keyword
		String_View s = get_next_word_no_skip(l->file, l->file_size, l->index, " \n\r\t\v\f");
		if (lexer_maybe(l, s, "v")) {
			tokens_append(&tokens, (Token){ITEM_v, 0});
			
			// expecting 3-4 floats
			for (int i = 0; i < 4; ++i) {
				if (is_end_of_line(l->file[l->index])) {
					if (i < 3) {
						// Error! At least three floats required.
						assert(!"Error while parsing. Expected at least three floats.");
						return tokens;
					} else {
						break;
					}
				}
				Next_Word nw = get_next_word(l->file, l->file_size, l->index, " \n\r\t\v\f");
				l->index += nw.offset;
				
				float f = string_to_float(nw.s.start, nw.s.length); // TODO: write an obj_string_to_float function
				tokens_append(&tokens, (Token){.type = ITEM_FLOAT, .value = {.f = f}});
				l->index += nw.s.length;
			}
		} else if (lexer_maybe(l, s, "vt")) {
			tokens_append(&tokens, (Token){ITEM_vt, 0});
			
			// expecting 1-3 floats
			for (int i = 0; i < 3; ++i) {
				if (is_end_of_line(l->file[l->index])) {
					if (i < 1) {
						// Error! At least one float required.
						assert(!"Error while parsing. Expected at least one float.");
						return tokens;
					} else {
						break;
					}
				}
				Next_Word nw = get_next_word(l->file, l->file_size, l->index, " \n\r\t\v\f");
				l->index += nw.offset;
				
				float f = string_to_float(nw.s.start, nw.s.length); // TODO: write an obj_string_to_float function
				tokens_append(&tokens, (Token){.type = ITEM_FLOAT, .value = {.f = f}});
				l->index += nw.s.length;
			}
		} else if (lexer_maybe(l, s, "vn")) {
			tokens_append(&tokens, (Token){ITEM_vn, 0});
			
			// expecting 3 floats
			for (int i = 0; i < 3; ++i) {
				if (is_end_of_line(l->file[l->index])) {
					if (i < 3) {
						// Error! Three floats required.
						assert(!"Error while parsing. Expected three floats.");
						return tokens;
					} else {
						break;
					}
				}
				Next_Word nw = get_next_word(l->file, l->file_size, l->index, " \n\r\t\v\f");
				l->index += nw.offset;
				
				float f = string_to_float(nw.s.start, nw.s.length); // TODO: write an obj_string_to_float function
				tokens_append(&tokens, (Token){.type = ITEM_FLOAT, .value = {.f = f}});
				l->index += nw.s.length;
			}
		} else if (lexer_maybe(l, s, "vp")) {
			tokens_append(&tokens, (Token){ITEM_vp, 0});
			
			// expecting 1-3 floats
			for (int i = 0; i < 3; ++i) {
				if (is_end_of_line(l->file[l->index])) {
					if (i < 1) {
						// Error! At least one float required.
						assert(!"Error while parsing. Expected at least one float.");
						return tokens;
					} else {
						break;
					}
				}
				Next_Word nw = get_next_word(l->file, l->file_size, l->index, " \n\r\t\v\f");
				l->index += nw.offset;
				
				float f = string_to_float(nw.s.start, nw.s.length); // TODO: write an obj_string_to_float function
				tokens_append(&tokens, (Token){.type = ITEM_FLOAT, .value = {.f = f}});
				l->index += nw.s.length;
			}
		} else if (lexer_maybe(l, s, "f")) {
			tokens_append(&tokens, (Token){ITEM_f, 0});
			
			// expecting a, a/b, a//c, a/b/c thingies
			// TODO: loop this, at least three times!
			while (l->index < l->file_size && !is_end_of_line(l->file[l->index])) {
				Next_Word nw = get_next_word(l->file, l->file_size, l->index, "/ \n\r\t\v\f");
				l->index += nw.offset;
				
				if (!is_obj_int(nw.s.start, nw.s.length)) {
					assert(!"Error while parsing! Expected an int.");
					return tokens;
				}
				
				int i = string_to_int(nw.s.start, nw.s.length);
				tokens_append(&tokens, (Token){.type = ITEM_INT, .value = {.i = i}});
				l->index += nw.s.length;
				// int
				
				s = get_next_word_no_skip(l->file, l->file_size, l->index, "0123456789 \n\r\t\v\f");
				if (lexer_maybe(l, s, "/")) {
					// int/
					tokens_append(&tokens, (Token){ITEM_SLASH, 0});
					if (l->file[l->index] == '/') {
						// int//
						s = get_next_word_no_skip(l->file, l->file_size, l->index, "0123456789 \n\r\t\v\f");
						tokens_append(&tokens, (Token){ITEM_SLASH, 0});
						l->index += s.length;
						
						s = get_next_word_no_skip(l->file, l->file_size, l->index, "/ \n\r\t\v\f");
						if (lexer_maybe_int(l, s)) {
							// int//int
							i = string_to_int(s.start, s.length);
							tokens_append(&tokens, (Token){ITEM_INT, .value = {.i = i}});
						} else {
							assert(!"Error while parsing! Expected an int.");
							return tokens;	
						}
					} else if (is_numeric(l->file[l->index]) || l->file[l->index] == '+' || l->file[l->index] == '-') {
						// int/int
						s = get_next_word_no_skip(l->file, l->file_size, l->index, "/ \n\r\t\v\f");
						i = string_to_int(s.start, s.length);
						tokens_append(&tokens, (Token){ITEM_INT, .value = {.i = i}});
						l->index += s.length;
						
						s = get_next_word_no_skip(l->file, l->file_size, l->index, "0123456789 \n\r\t\v\f");
						if (lexer_maybe(l, s, "/")) {
							// int/int/
							tokens_append(&tokens, (Token){ITEM_SLASH, 0});
							s = get_next_word_no_skip(l->file, l->file_size, l->index, "/ \n\r\t\v\f");
							if (lexer_maybe_int(l, s)) {
								// int/int/int
								i = string_to_int(s.start, s.length);
								tokens_append(&tokens, (Token){ITEM_INT, .value = {.i = i}});
							} else {
								assert(!"Error while parsing! Expected an int.");
								return tokens;		
							}
						}
					} else {
						// error
						assert(!"Error while parsing! Expected a '/' or an int.");
						return tokens;
					}
				}
			}
		} else if (lexer_maybe(l, s, "o")) {
			tokens_append(&tokens, (Token){ITEM_o, 0});
			
			// expecting identifier
			Next_Word nw = get_next_word(l->file, l->file_size, l->index, " \n\r\t\v\f");
			l->index += nw.offset;
			
			tokens_append(&tokens, (Token){.type = ITEM_IDENTIFIER, .value = {.s = nw.s}});
			l->index += nw.s.length;
		} else {
			// Error! Expected keyword.
			assert(!"Error while parsing. Keyword expected.");
			return tokens;
		}
	}
				
	return tokens;
}

//
// Parser

typedef struct Parser {
	Tokens tokens;
	
	bool done;
	
	// TODO...
} Parser;

Token next(Parser *p) {
	return STUB(Token);
}

Token peek(Parser *p) {
	return STUB(Token);
}

bool expect(Parser *p, Token expected) {
	Token t = next(p);
	if (t.type == expected.type) {
		return false;		
	}
	return true;
}


Scene obj_parse(byte *obj_file_data, int file_size) {
	assert(obj_file_data && file_size > 0);
	
	Scene s = {0};
	
	Lexer lexer = {(char *)obj_file_data, file_size};
	Tokens tokens = tokenize(&lexer);
	
	Parser parser = {tokens};
	while (!parser.done) {
		Token t = next(&parser);
		
		switch(t.type) {
		case ITEM_v: 
			
			break;
		}
	}
	
	return s;
}