#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_CLAMP_TO_EDGE                  0x812F

typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;

#define GL_DEBUG_CALLBACK(name) void APIENTRY name(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
typedef GL_DEBUG_CALLBACK(GLDEBUGPROC);

typedef GLuint type_glCreateShader(GLenum shaderType);
typedef void   type_glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
typedef void   type_glCompileShader(GLuint shader);
typedef void   type_glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
typedef GLuint type_glCreateProgram(void);
typedef void   type_glAttachShader(GLuint program, GLuint shader);
typedef void   type_glLinkProgram(GLuint program);
typedef void   type_glGetProgramiv(GLuint program, GLenum pname, GLint *params);
typedef void   type_glDetachShader(GLuint program, GLuint shader);
typedef void   type_glDeleteShader(GLuint shader);
typedef void   type_glUseProgram(GLuint program);
typedef void   type_glGenBuffers(GLsizei n, GLuint * buffers);
typedef void   type_glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void   type_glNamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
typedef void   type_glGenVertexArrays(GLsizei n, GLuint *arrays);
typedef void   type_glBindVertexArray(GLuint array);
typedef void   type_glBindBuffer(GLenum target, GLuint buffer);
typedef void   type_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
typedef void   type_glEnableVertexAttribArray(GLuint index);
typedef GLint  type_glGetUniformLocation(GLuint program, const GLchar *name);
typedef void   type_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void   type_glProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void   type_glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void   type_glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void   type_glGenSamplers(GLsizei n, GLuint *samplers);
typedef void   type_glDeleteSamplers(GLsizei n, const GLuint * samplers);
typedef void   type_glBindSampler(GLuint unit, GLuint sampler);
typedef void   type_glSamplerParameteri(GLuint sampler, GLenum pname, GLint param);
typedef void   type_glActiveTexture(GLenum texture);
typedef void   type_glProgramUniform1i(GLuint program, GLint location, GLint v0);
typedef void   type_glDebugMessageCallback(GLDEBUGPROC *Callback, const void *UserParam);
typedef void   type_glValidateProgram(GLuint program);
typedef void   type_glUniform3fv(GLint location, GLsizei count, const GLfloat *value);
typedef void   type_glUniform1i(GLint location, GLint v0);

#define opengl_function(name) type_##name *name

struct opengl {
    GLuint default_program;
    GLuint vertex_buffer;
    GLuint index_buffer;
    
    bool supports_swap_control;
    bool supports_swap_control_tear;
    
    opengl_function(glDebugMessageCallback);
    opengl_function(glCreateShader);
    opengl_function(glShaderSource);
    opengl_function(glCompileShader);
    opengl_function(glGetShaderiv);
    opengl_function(glCreateProgram);
    opengl_function(glAttachShader);
    opengl_function(glLinkProgram);
    opengl_function(glGetProgramiv);
    opengl_function(glDetachShader);
    opengl_function(glDeleteShader);
    opengl_function(glUseProgram);
    opengl_function(glGenBuffers);
    opengl_function(glBufferData);
    opengl_function(glNamedBufferData);
    opengl_function(glGenVertexArrays);
    opengl_function(glBindVertexArray);
    opengl_function(glBindBuffer);
    opengl_function(glVertexAttribPointer);
    opengl_function(glEnableVertexAttribArray);
    opengl_function(glGetUniformLocation);
    opengl_function(glUniformMatrix4fv);
    opengl_function(glProgramUniformMatrix4fv);
    opengl_function(glGetProgramInfoLog);
    opengl_function(glGetShaderInfoLog);
    opengl_function(glGenSamplers);
    opengl_function(glDeleteSamplers);
    opengl_function(glBindSampler);
    opengl_function(glSamplerParameteri);
    opengl_function(glActiveTexture);
    opengl_function(glProgramUniform1i);
    opengl_function(glValidateProgram);
    opengl_function(glUniform3fv);
    opengl_function(glUniform1i);
};

GL_DEBUG_CALLBACK(opengl_debug_callback) {
    if(severity == GL_DEBUG_SEVERITY_HIGH) {
        wchar_t *error_message = (wchar_t *)message;
        
        OutputDebugString(L"OPENGL: ");
        OutputDebugString(error_message);
        OutputDebugString(L"\n");
        
        assert(!"OpenGL Error occurred.\n");
    }
}

void compile_default_program(struct opengl *opengl) {
    int vertex_code_size[1];
    GLuint vertex_shader = opengl->glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vertex_code = (const GLchar *)read_entire_file(L"../shaders/default.vert", vertex_code_size);
    opengl->glShaderSource(vertex_shader, 1, &vertex_code, vertex_code_size);
    opengl->glCompileShader(vertex_shader);
    
    int fragment_code_size[1];
    GLuint fragment_shader = opengl->glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fragment_code = (const GLchar *)read_entire_file(L"../shaders/default.frag", fragment_code_size);
    opengl->glShaderSource(fragment_shader, 1, &fragment_code, fragment_code_size);
    opengl->glCompileShader(fragment_shader);
    
    GLuint program = opengl->glCreateProgram();
    opengl->glAttachShader(program, vertex_shader);
    opengl->glAttachShader(program, fragment_shader);
    opengl->glLinkProgram(program);
    
    opengl->glDetachShader(program, vertex_shader);
    opengl->glDetachShader(program, fragment_shader);
    
    opengl->glValidateProgram(program);
    GLint linked = false;
    opengl->glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked) {
        char vertex_errors[1024] = {0};
        char fragment_errors[1024] = {0};
        char program_errors[1024] = {0};
        
        opengl->glGetShaderInfoLog(vertex_shader, sizeof(vertex_errors), NULL, vertex_errors);
        opengl->glGetShaderInfoLog(fragment_shader, sizeof(fragment_errors), NULL, fragment_errors);
        opengl->glGetProgramInfoLog(program, sizeof(program_errors), NULL, program_errors);
        
        OutputDebugStringA("Vertex Shader Errors:\n");
        OutputDebugStringA(vertex_errors);
        OutputDebugStringA("Fragment Shader Errors:\n");
        OutputDebugStringA(fragment_errors);
        OutputDebugStringA("Program Linking Errors:\n");
        OutputDebugStringA(program_errors);
        
        assert(!"Shader validation failed!\n");

        ExitProcess(1);
    }

    opengl->glDeleteShader(vertex_shader);
    opengl->glDeleteShader(fragment_shader);
    
    opengl->default_program = program;
}

void opengl_init(struct opengl *opengl)
{
#if defined(_DEBUG)
    if(opengl->glDebugMessageCallback) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        opengl->glDebugMessageCallback(opengl_debug_callback, NULL);
    }
#endif
    
    compile_default_program(opengl);
    
    GLuint vertex_array;
    opengl->glGenVertexArrays(1, &vertex_array);
    opengl->glBindVertexArray(vertex_array);
    
    opengl->glGenBuffers(1, &opengl->vertex_buffer);
    opengl->glBindBuffer(GL_ARRAY_BUFFER, opengl->vertex_buffer);
    
    opengl->glGenBuffers(1, &opengl->index_buffer);
    opengl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, opengl->index_buffer);
}

void opengl_draw_scene(struct opengl *opengl, int window_width, int window_height, Scene *scene, view_control_t *control) {
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    opengl->glNamedBufferData(opengl->vertex_buffer, scene->vertex_count * sizeof(*scene->vertex_array), scene->vertex_array, GL_STATIC_DRAW);
    opengl->glNamedBufferData(opengl->index_buffer, scene->index_count * sizeof(*scene->index_array), scene->index_array, GL_STATIC_DRAW);
    
    opengl->glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Textured_Vertex), (void *)offsetof(Textured_Vertex, position));
    opengl->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Textured_Vertex), (void *)offsetof(Textured_Vertex, uv));
    opengl->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Textured_Vertex), (void *)offsetof(Textured_Vertex, normal));
    opengl->glEnableVertexAttribArray(0);
    opengl->glEnableVertexAttribArray(1);
    opengl->glEnableVertexAttribArray(2);
    
    opengl->glUseProgram(opengl->default_program);
    
    vec3 center;
    mat4 model, view, projection, mvp;
    
    cglm_vec3_add(control->position, control->forward, center);

    cglm_mat4_identity(model);
    cglm_look_at(control->position, center, control->up, view);
    cglm_perspective(control->fov, (float)window_height / (float)window_width, 0.1f, 100.0f, projection);

    cglm_mat4_mul3(projection, view, model, mvp);
    cglm_mat4_mul(projection, view, mvp);
    
    opengl->glUniformMatrix4fv(0, 1, GL_FALSE, (float *)mvp);
    //opengl->glUniform1i(1, 0);
    
    glViewport(0, 0, window_width, window_height);
    
    glDrawElements(GL_TRIANGLES, scene->index_count, GL_UNSIGNED_INT, 0);
}
