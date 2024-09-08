enum Lexer_State {
	STATE_START,
	STATE_WHITESPACE,
	STATE_WORD_START,
	STATE_NUMBER,
	STATE_SLASH,
	STATE_KEYWORD_OR_IDENTIFIER,
};

enum Token_Type {
	ITEM_NONE = -1,
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

//
// Lexer 

typedef struct Lexer {
	char *file;
	int file_size;
	
	int index;
	int state;
	
	bool done;
} Lexer;

Token next(Lexer *l) {
	Token t = {0};
	
	int word_start = 0;
	int word_length = 0;
	
	bool dot = false;
	
	while (l->index < l->file_size) { 
		char c = l->file[l->index];

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
			if (c == '-' || c == '+' || c == '.' || is_numeric(c)) { // TODO!
				l->state = STATE_NUMBER;
			} else if (is_alphabetic(c)) {
				l->state = STATE_KEYWORD_OR_IDENTIFIER;
			}
			break;
		case STATE_NUMBER:
			if (is_numeric(c)) {
				l->index++;
			} else if (c == '.') {
				if (dot) {
					// TODO: Error, 2 dots in a number!
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
				t.type = ITEM_NUMBER;
				t.value = (String_View){l->file + word_start, word_length};
				goto done;
			} else {
				// TODO: Error
			}
			break;
		case STATE_KEYWORD_OR_IDENTIFIER:
			// TODO
			break;
		case STATE_SLASH:
			// TODO
			break;
		}
	}
	
done:
	return t;
}

Token peek(Lexer *lexer) {
	return STUB(Token);
}

bool expect(Lexer *lexer, Token expected) {
	Token t = next(lexer);
	if (t.type == expected.type) {
		return false;		
	}
	return true;
}

//
// Parser

Scene obj_parse(byte *obj_file_data, int file_size) {
	if (!obj_file_data || file_size == 0) {
		return STUB(Scene);
	}
	
	Scene s = {0};
	
	Lexer lexer = {(char *)obj_file_data, file_size};
	while (!lexer.done) {
		Token t = next(&lexer);
		
		switch(t.type) {
		case ITEM_v: 
			
			break;
		}
	}
	
	return s;
}