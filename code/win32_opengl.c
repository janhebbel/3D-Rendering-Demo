#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_PIXEL_TYPE_ARB                      0x2013

#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_FULL_ACCELERATION_ARB               0x2027

#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS_ARB                    0x2023

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001

typedef BOOL wgl_choose_pixel_format_arb(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef HGLRC wgl_create_context_attribs_arb(HDC hDC, HGLRC hshareContext, const int *attribList);
typedef const char *wgl_get_extensions_string_ext(void);
typedef const char *wgl_get_extensions_string_arb(HDC hdc);
typedef BOOL wgl_swap_interval_ext(int interval);

wgl_choose_pixel_format_arb *wglChoosePixelFormatARB;
wgl_create_context_attribs_arb *wglCreateContextAttribsARB;
wgl_get_extensions_string_ext *wglGetExtensionsStringEXT;
wgl_get_extensions_string_arb *wglGetExtensionsStringARB;
wgl_swap_interval_ext *wglSwapIntervalEXT;

void set_pixel_format(HDC window_dc)
{
    int suggested_pixel_format_index = 0;
    UINT suitable_formats = 0;
    if(wglChoosePixelFormatARB)
    {
        // Set Pixel Format
        int attribute_list[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,     24,
            WGL_ALPHA_BITS_ARB,     8,
            WGL_DEPTH_BITS_ARB,     24,
            WGL_STENCIL_BITS_ARB,   8,
            0
        };
        
        wglChoosePixelFormatARB(window_dc, attribute_list, NULL, 1, &suggested_pixel_format_index, &suitable_formats);
    }
    
    if(!suitable_formats)
    {
        PIXELFORMATDESCRIPTOR desired_pixel_format = {0};
        
        desired_pixel_format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        desired_pixel_format.nVersion = 1;
        desired_pixel_format.iPixelType = PFD_TYPE_RGBA;
        desired_pixel_format.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
        desired_pixel_format.cColorBits = 32;
        desired_pixel_format.cAlphaBits = 8;
        desired_pixel_format.cDepthBits = 24;
        desired_pixel_format.cStencilBits = 8;
        desired_pixel_format.iLayerType = PFD_MAIN_PLANE;
        
        suggested_pixel_format_index = ChoosePixelFormat(window_dc, &desired_pixel_format);
    }
    
    PIXELFORMATDESCRIPTOR suggested_pixel_format;
    
    DescribePixelFormat(window_dc, suggested_pixel_format_index, sizeof(suggested_pixel_format), &suggested_pixel_format);
    SetPixelFormat(window_dc, suggested_pixel_format_index, &suggested_pixel_format);
}

void load_wgl_extensions(opengl_t *opengl)
{
    WNDCLASS window_class = {0};
    window_class.lpfnWndProc = DefWindowProc;
    window_class.hInstance = GetModuleHandle(0);
    window_class.lpszClassName = L"HandmadeGameWGLLoader";
    
    if(RegisterClass(&window_class))
    {
        HWND window = CreateWindowEx(
                                     0,
                                     window_class.lpszClassName,
                                     L"Chadge",
                                     0,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     0,
                                     0,
                                     window_class.hInstance,
                                     0);
        
        HDC window_dc = GetDC(window);
        
        set_pixel_format(window_dc);
        
        HGLRC opengl_rc = wglCreateContext(window_dc);
        
        if(wglMakeCurrent(window_dc, opengl_rc))
        {
            wglChoosePixelFormatARB = (wgl_choose_pixel_format_arb *)wglGetProcAddress("wglChoosePixelFormatARB");
            wglCreateContextAttribsARB = (wgl_create_context_attribs_arb *)wglGetProcAddress("wglCreateContextAttribsARB");
            wglGetExtensionsStringEXT = (wgl_get_extensions_string_ext *)wglGetProcAddress("wglGetExtensionsStringEXT");
            wglGetExtensionsStringARB = (wgl_get_extensions_string_arb *)wglGetProcAddress("wglGetExtensionsStringARB");
            wglSwapIntervalEXT = (wgl_swap_interval_ext *)wglGetProcAddress("wglSwapIntervalEXT");
            
            if(wglGetExtensionsStringEXT)
            {
                char *extensions = (char *)wglGetExtensionsStringEXT();
                char *at = extensions;
                while(*at)
                {
                    while(*at == ' ') ++at;
                    char *end = at;
                    while(*end && *end != ' ') ++end;
                    
                    int count = (int)(end - at);
                    
                    if(0) {}
                    else if(strings_are_equal_1l(count, at, "WGL_EXT_swap_control")) opengl->supports_swap_control = true;
                    else if(strings_are_equal_1l(count, at, "WGL_EXT_swap_control_tear")) opengl->supports_swap_control_tear = true;
                    
                    at = end;
                }
            }
            
            wglMakeCurrent(0, 0);
        }
        
        wglDeleteContext(opengl_rc);
        ReleaseDC(window, window_dc);
        DestroyWindow(window);
    }
}

opengl_t *platform_opengl_init(HDC renderer_dc, int max_quad_count_per_frame)
{
    opengl_t *opengl = malloc(sizeof(*opengl));
    load_wgl_extensions(opengl);
    set_pixel_format(renderer_dc);
    
    int max_vertex_count = 4 * max_quad_count_per_frame;
    int max_index_count  = 6 * max_quad_count_per_frame;
    max_vertex_count, max_index_count; // TODO!
    
    int opengl_attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    
    HGLRC opengl_rc = 0;
    if(wglCreateContextAttribsARB)
    {
        opengl_rc = wglCreateContextAttribsARB(renderer_dc, 0, opengl_attributes);
    }
    
    if(!opengl_rc)
    {
        opengl_rc = wglCreateContext(renderer_dc);
    }
    
    if(wglMakeCurrent(renderer_dc, opengl_rc))
    {
        
#define get_opengl_function(name) opengl->name = (type_##name *)wglGetProcAddress(#name);
        
        get_opengl_function(glDebugMessageCallback);
        get_opengl_function(glCreateShader);
        get_opengl_function(glShaderSource);
        get_opengl_function(glCompileShader);
        get_opengl_function(glGetShaderiv);
        get_opengl_function(glCreateProgram);
        get_opengl_function(glAttachShader);
        get_opengl_function(glLinkProgram);
        get_opengl_function(glGetProgramiv);
        get_opengl_function(glDetachShader);
        get_opengl_function(glDeleteShader);
        get_opengl_function(glUseProgram);
        get_opengl_function(glGenBuffers);
        get_opengl_function(glBufferData);
        get_opengl_function(glNamedBufferData);
        get_opengl_function(glGenVertexArrays);
        get_opengl_function(glBindVertexArray);
        get_opengl_function(glBindBuffer);
        get_opengl_function(glVertexAttribPointer);
        get_opengl_function(glEnableVertexAttribArray);
        get_opengl_function(glGetUniformLocation);
        get_opengl_function(glUniformMatrix4fv);
        get_opengl_function(glProgramUniformMatrix4fv);
        get_opengl_function(glGetProgramInfoLog);
        get_opengl_function(glGetShaderInfoLog);
        get_opengl_function(glGenSamplers);
        get_opengl_function(glDeleteSamplers);
        get_opengl_function(glBindSampler);
        get_opengl_function(glSamplerParameteri);
        get_opengl_function(glActiveTexture);
        get_opengl_function(glProgramUniform1i);
        get_opengl_function(glValidateProgram);
        get_opengl_function(glUniform3fv);
        get_opengl_function(glUniform1i);
        
        opengl_init(opengl);
    }
    
    return(opengl);
}
