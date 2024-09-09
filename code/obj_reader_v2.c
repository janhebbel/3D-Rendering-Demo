enum Lexer_State {
	STATE_START,
	STATE_WHITESPACE,
	STATE_WORD_START,
	STATE_NUMBER,
	STATE_SLASH,
	STATE_KEYWORD_OR_IDENTIFIER,
	STATE_IDENTIFIER
};

enum Token_Type {
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

typedef struct Token {
	enum Token_Type type;
	String_View value;
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

// TODO: fix /n situation

typedef struct Lexer {
	char *file;
	int file_size;
	
	int index;
	int state;
} Lexer;

// advances the internal index if s and string are equal
bool lexer_maybe(Lexer *l, String_View s, char *string) {
	if (strings_are_equal_1l(s.length, s.start, string)) {
		l->index += s.length;
		return true;
	}
	return false;
}

String_View get_next_word(char *chars, int size, int offset, char *delim) {
	String_View s = {chars + offset};
	int length = 0;
	while (length + offset < size) {
		int d_index = 0;
		while (delim[d_index]) {
			if (delim[d_index] == chars[offset + length]) {
				s.length = length;
				return s;
			} else {
				d_index++;
			}
		}
		length++;
	}
	return s;
}

Tokens tokenize(Lexer *l) {
	// Using heuristic that assumes #tokens < file_size to avoid reallocations.
	Tokens tokens = tokens_init(l->file_size);
	
	// int word_start = 0;
	// int word_length = 0;
	
	// bool dot = false;
	
	bool done = false;
	
	// scanning line by line
	while (!done) {
		// skip any whitespaces
		while (l->index < l->file_size && is_whitespace(l->file[l->index]))
			l->index++;
		
		// scan one line
		while (l->index < l->file_size && !is_end_of_line(l->file[l->index])) {
			//char c = l->file[l->index];
			
			// scan keyword
			String_View s = get_next_word(l->file, l->file_size, l->index, " \n\r\t\v\f");
			if (lexer_maybe(l, s, "v")) {
				tokens_append(&tokens, (Token){ITEM_v, 0});
				// TODO: scan 3 or 4 floats
			} else if (lexer_maybe(l, s, "vt")) {
				tokens_append(&tokens, (Token){ITEM_vt, 0});
				// TODO: scan 1-3 floats
			} else if (lexer_maybe(l, s, "vn")) {
				tokens_append(&tokens, (Token){ITEM_vn, 0});
				// TODO: scan 3 floats
			} else if (lexer_maybe(l, s, "vp")) {
				tokens_append(&tokens, (Token){ITEM_vp, 0});
				// TODO: scan ??
			} else if (lexer_maybe(l, s, "f")) {
				tokens_append(&tokens, (Token){ITEM_f, 0});
				// TODO: scan whatever the fuck this is
			} else if (lexer_maybe(l, s, "o")) {
				tokens_append(&tokens, (Token){ITEM_o, 0});
				// TODO: scan identifier
			} else {
				// Error! Expected keyword.
				assert(!"Error while parsing. Keyword expected.");
				return tokens;
			}
		}
		
		done = l->index == l->file_size - 1;
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