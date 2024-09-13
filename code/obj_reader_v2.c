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
	Arena *arena;
	Token *items;
	int count;
	int size;
} Tokens;

#define INITIAL_TOKENS_AMOUNT 128

Tokens tokens_init(Arena *a) {
	Tokens ts = {0};

	int size = INITIAL_TOKENS_AMOUNT * sizeof(*ts.items);
	
	ts.arena = a;
	ts.items = arena_alloc(a, size);
	ts.count = 0;
	ts.size = size;

	return ts;
}

void tokens_append(Tokens *ts, Token t) {
	if (ts->count * sizeof(*ts->items) >= ts->size) {
		ts->items = arena_resize(ts->arena, ts->items, ts->size, ts->size * 2);
		ts->size *= 2;
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

	int vcount;
	int vtcount;
	int vncount;
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

Tokens tokenize(Lexer *l, Arena *a) {
	Tokens tokens = tokens_init(a);
	
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
			l->vcount++;
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

				// TODO: test if float
				float f = string_to_float(nw.s.start, nw.s.length);
				tokens_append(&tokens, (Token){.type = ITEM_FLOAT, .value = {.f = f}});
				l->index += nw.s.length;
			}
		} else if (lexer_maybe(l, s, "vt")) {
			l->vtcount++;
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

				// TODO: test if float
				float f = string_to_float(nw.s.start, nw.s.length);
				tokens_append(&tokens, (Token){.type = ITEM_FLOAT, .value = {.f = f}});
				l->index += nw.s.length;
			}
		} else if (lexer_maybe(l, s, "vn")) {
			l->vncount++;
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

				// TODO: test if float
				float f = string_to_float(nw.s.start, nw.s.length);
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

				// TODO: test if float
				float f = string_to_float(nw.s.start, nw.s.length);
				tokens_append(&tokens, (Token){.type = ITEM_FLOAT, .value = {.f = f}});
				l->index += nw.s.length;
			}
		} else if (lexer_maybe(l, s, "f")) {
			tokens_append(&tokens, (Token){ITEM_f, 0});
			
			// expecting a, a/b, a//c, a/b/c thingies
			int poly_corner_count = 0;
			while (l->index < l->file_size && !is_end_of_line(l->file[l->index])) {
				poly_corner_count++;
				
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

			if (poly_corner_count != 3) {
				assert(!"Only triangles supported!");
				return tokens;
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

typedef struct OBJ_Vertex_Attributes {
	// v
	vec4 *p;
	int pcount;
	// vt
	vec2 *uv;
	int uvcount;
	// vn
	vec3 *n;
	int ncount;
} OBJ_Vertex_Attributes;

bool textured_vertex_compare(Textured_Vertex *v1, Textured_Vertex *v2) {
	return memcmp(v1, v2, sizeof(*v1)) == 0;
}

// look for vertex in the scene's vertex array,
// if one like it exists then return its index,
// else return a value smaller 0
int obj_get_index(Scene *s, Textured_Vertex *vertex) {
	for (int i = 0; i < s->vertex_count; ++i) {
		if (textured_vertex_compare(&s->vertex_array[i], vertex)) {
			return i;
		}
	}

	return -1;
}

Scene obj_parse(byte *obj_file_data, int file_size, Arena *scratch) {
	assert(obj_file_data && file_size > 0);
	
	Scene s = {0};

	// lexing
	Lexer lexer = {(char *)obj_file_data, file_size};
	Tokens tokens = tokenize(&lexer, scratch);

	// parsing
	OBJ_Vertex_Attributes vertices = {0};
	vertices.p  = arena_alloc(scratch, lexer.vcount  * sizeof(*vertices.p));
	vertices.uv = arena_alloc(scratch, lexer.vtcount * sizeof(*vertices.uv));
	vertices.n  = arena_alloc(scratch, lexer.vncount * sizeof(*vertices.n));

	for (int i = 0; i < tokens.count; ++i) {
		switch(tokens.items[i].type) {
		case ITEM_v: {
			i++;
			
			vec4 pos;
			int j;
			for (j = 0; j < 3; ++j) {
				assert(i + j < tokens.count);
				assert(tokens.items[i + j].type == ITEM_FLOAT);
				
				pos[j] = tokens.items[i + j].value.f;
			}

			assert(i + j < tokens.count);
			if (tokens.items[i + j].type == ITEM_FLOAT) {
				pos[j] = tokens.items[i + j].value.f;
			} else {
				pos[j] = 1.0f;
			}

			cglm_vec4_set(vertices.p[vertices.pcount++], pos); // vertices.p[vertices.pcount++] = pos

			i += j - 1;
			break;
		}
			
		case ITEM_vt: {
			i++;

			vec2 uv;
			int j;
			for (j = 0; j < 2; ++j) {
				assert(i + j < tokens.count);

				if (tokens.items[i + j].type != ITEM_FLOAT) {
					break;
				}
				
				uv[j] = tokens.items[i + j].value.f;
			}

			assert(j == 2); // only texture coordinates with u and v component supported

			cglm_vec2_set(vertices.uv[vertices.uvcount++], uv);

			i += j - 1;
			break;
		}
			
		case ITEM_vn: {
			i++;

			vec3 n;
			int j;
			for (j = 0; j < 3; ++j) {
				assert(i + j < tokens.count);
				assert(tokens.items[i+j].type == ITEM_FLOAT);

				n[j] = tokens.items[i+j].value.f;
			}

			cglm_vec3_set(vertices.n[vertices.ncount++], n);

			i += j - 1;
			break;
		}
			
		case ITEM_vp: {
			// TODO
			break;
		}
			
		case ITEM_f: {
			Textured_Vertex vertex;
			memset(&vertex, 0, sizeof(Textured_Vertex)); // NOTE: set 0 here because of compare function
			
			int pidx = 0, uvidx = 0, nidx = 0;
			
			// TODO: acquire pidx, uvidx, and nidx
			
			cglm_vec4_set(vertex.position, vertices.p[pidx]);
			cglm_vec2_set(vertex.uv, vertices.uv[uvidx]);
			cglm_vec3_set(vertex.normal, vertices.n[nidx]);
			
			int index = obj_get_index(&s, &vertex);
			if (index < 0) {
				// vertex not found, place it in the back of vertex array
				// index := vertices_append(vertices, vertex);
			}
			break;
		}
			
		case ITEM_o: {
			// TODO
			i++;
			break;
		}

		// case ITEM_FLOAT: case ITEM_INT: case ITEM_IDENTIFIER: case ITEM_SLASH: {
		// 	assert(!"These should be handled in the keyword cases.!");
		// 	return s;
		// }

		// default:
		// 	assert(!"Shouldn't be reachable.");
		// 	return s;
		}
	}

	arena_free_all(scratch);

	return s;
}
