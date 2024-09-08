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
	int type;
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

typedef struct Lexer {
	char *file;
	int file_size;
	
	int index;
	int state;
	
	bool done;
} Lexer;

char lexer_peek(Lexer *l) {
	return (l->index + 1 < l->file_size) ? l->file[l->index] : 0;
}

char lexer_consume(Lexer *l) {
	return (l->index + 1 < l->file_size) ? l->file[l->index++] : 0;
}

Tokens tokenize(Lexer *l) {
	// Using heuristic that assumes #tokens < file_size to avoid reallocations.
	Tokens tokens = tokens_init(l->file_size);
	
	int word_start = 0;
	int word_length = 0;
	
	bool dot = false;
	
	bool done = false;
	
	while (!done) { 
		char c = lexer_peek(l);

		switch(l->state) {
		case STATE_START:
			if (is_whitespace(c)) {
				l->state = STATE_WHITESPACE;
			} else {
				l->state = STATE_WORD_START;
			}
			break;
		case STATE_WHITESPACE:
			if (is_whitespace(c)) {
				l->index++;
			} else {
				l->state = STATE_WORD_START;
			}
			break;
		case STATE_WORD_START:
			word_start = l->index;
			// TODO: unfinished if-else-block
			if (c == '-' || c == '+' || c == '.' || is_numeric(c)) {
				l->state = STATE_NUMBER;
			} else if (is_alphabetic(c)) {
				l->state = STATE_KEYWORD_OR_IDENTIFIER;
			} else if (c == '/') {
				l->state = STATE_SLASH;
			}
			break;
		case STATE_NUMBER:
			if (is_numeric(c)) {
				l->index++;
			} else if (c == '.') {
				if (dot) {
					assert(!"Error while parsing. Encountered two decimal points while parsing a number.");
					return tokens;
				} else {
					l->index++;
					dot = true;
				}
			} else if (is_whitespace(c) || c == '/') { // number is done parsing
				word_length = l->index - word_start;
				if(is_whitespace(c)) {
					l->state = STATE_WHITESPACE;
				} else if (c == '/') {
					l->state = STATE_SLASH;
				}
				tokens_append(&tokens, (Token){ITEM_NUMBER, (String_View){l->file + word_start, word_length}});
			} else {
				assert(!"Error while parsing. Encountered illegal character while parsing a number.");
				return tokens;
			}
			break;
		case STATE_KEYWORD_OR_IDENTIFIER:
			if (c == 'v' || c == 'f' || c == 'o') { // TODO: not all beginning characters for every keyword listed
				
			} else { // Not a keyword but an identifier
				l->state = STATE_IDENTIFIER;
			}
			break;
		case STATE_SLASH:
			l->index++; // FIXME: this coorect?
			tokens_append(&tokens, (Token){ITEM_NUMBER, (String_View){l->file + word_start, word_length}});
			// TODO: which state?
			break;
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