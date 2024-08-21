#include <windows.h>
#include <GL/gl.h>

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stddef.h>
#include <wchar.h>

#define CGLM_IMPLEMENTATION
#include "cglm.h"
#include "types.h"
#include "renderer.h"
#include "settings.h"
#define CHC_DARRAY_IMPLEMENTATION
#include "darray.h"

#include "utils.c"
#include "win32_utils.c"
#include "input.c"
#include "handle_input.c"
#include "opengl_renderer.c"
#include "win32_opengl.c"
#include "bitmap.c"
#include "obj_reader.c"

int global_running = true;

void toggle_fullscreen(HWND window) {
    static DWORD prev_window_style;
    static WINDOWPLACEMENT prev_window_placement = { sizeof(prev_window_placement) };
    
    DWORD style = GetWindowLong(window, GWL_STYLE);
    DWORD fullscreen_style = WS_POPUP|WS_VISIBLE;
    
    RECT window_rect;
    GetWindowRect(window, &window_rect);
    
    MONITORINFO monitor_info = { sizeof(monitor_info) };
    GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &monitor_info);
    
    bool windowed = !(monitor_info.rcMonitor.left == window_rect.left && monitor_info.rcMonitor.right == window_rect.right && 
                      monitor_info.rcMonitor.top == window_rect.top && monitor_info.rcMonitor.bottom == window_rect.bottom);
    
    if(windowed) {
        prev_window_style = style;
        GetWindowPlacement(window, &prev_window_placement);
        
        SetWindowLong(window, GWL_STYLE, fullscreen_style);
        SetWindowPos(window, HWND_TOP, 
                     monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
                     monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
                     monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
                     SWP_NOOWNERZORDER|SWP_FRAMECHANGED);
    }
    else {
        SetWindowLong(window, GWL_STYLE, prev_window_style);
        SetWindowPlacement(window, &prev_window_placement);
        SetWindowPos(window, NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_FRAMECHANGED);
    }
}

LRESULT CALLBACK main_window_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    LRESULT lresult = 1;

    switch(message)
    {
        case WM_ACTIVATEAPP: {
            clear_keyboard_state();
            break;
        }
        
        case WM_CLOSE: {
            global_running = false;
            break;
        }
        
        case WM_DESTROY: {
            global_running = false;
            break;
        }
        
        case WM_QUIT: {
            global_running = false;
            break;
        }
        
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            int key = LOWORD(wparam);
            
            int was_down = (lparam & (1 << 30)) != 0;
            int down = (lparam & (1 << 31)) == 0;
            
            if(was_down != down) {
                int alt_down = (lparam & (1 << 29)) != 0;
                
                if(key == VK_F11 && down) {
                    toggle_fullscreen(window);
                }
                
                if(key == VK_F4 && alt_down && down) {
                    global_running = false;
                }
                
                key_event(key, down);
            }
            
            break;
        }

        case WM_LBUTTONDOWN: {
            key_event(MOUSE_LBUTTON, 1);
            break;
        }

        case WM_LBUTTONUP: {
            key_event(MOUSE_LBUTTON, 0);
            break;
        }

        case WM_RBUTTONDOWN: {
            key_event(MOUSE_RBUTTON, 1);

            ShowCursor(FALSE);
            GetCursorPos((POINT *)&global_initial_cursor_position);
            break;
        }
        
        case WM_RBUTTONUP: {
            key_event(MOUSE_RBUTTON, 0);

            ShowCursor(TRUE);
            break;			
        }
        
        case WM_MOUSEWHEEL: {
            global_scroll_update.yoffset = -(GET_WHEEL_DELTA_WPARAM(wparam) / (double)WHEEL_DELTA);
            global_scroll_update.updated = 1;
            break;
        }
        
        default: {
            lresult = DefWindowProc(window, message, wparam, lparam);
            break;
        }
    }
    
    return(lresult);
}

void process_pending_messages(void) {
    MSG message;
    while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prev_instance, PWCHAR cmd_line, int cmd_show) {
    WNDCLASS window_class = {0};
    
    window_class.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    window_class.lpfnWndProc = main_window_procedure;
    window_class.hInstance = instance;
    window_class.hCursor = LoadCursor(0, IDC_ARROW);
    // WindowClass.hIcon;
    window_class.lpszClassName = L"ChadgeWindowClass";

    if(RegisterClass(&window_class)) {
        DWORD window_style = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
        DWORD ex_window_style = 0;
        
        RECT rect = {0};
        rect.left = 0;
        rect.right = WINDOW_DEFAULT_WIDTH;
        rect.top = 0;
        rect.bottom = WINDOW_DEFAULT_HEIGHT;
        AdjustWindowRectEx(&rect, window_style, 0, ex_window_style);
        
        HWND window = CreateWindowEx(0,
                                     window_class.lpszClassName,
                                     L"Chadge",
                                     window_style,
                                     CW_USEDEFAULT, CW_USEDEFAULT,
                                     rect.right - rect.left,
                                     rect.bottom - rect.top,
                                     NULL,
                                     NULL,
                                     instance,
                                     NULL);
        
        if(window) {
            HDC renderer_dc = GetDC(window);

            struct opengl *opengl = platform_opengl_init(renderer_dc, 1 << 20);
            if(opengl == NULL) {
                MessageBox(NULL, L"Failed to initialize OpenGL.", L"Error", MB_OK|MB_ICONERROR);
                ExitProcess(1);
            }

            if(wglSwapIntervalEXT) {
#if defined(_DEBUG)
                wglSwapIntervalEXT(0); // Disable vsync.
#else
                if(opengl->supports_swap_control_tear) {
                    wglSwapIntervalEXT(-1); // Enable adaptive sync.
                }
                else {
                    wglSwapIntervalEXT(1); // Enable vsync.
                }
#endif
            }

            struct scene cube = {0};
            cube.vertex_array = (struct textured_vertex[5]) {
                {{-1.0f, -1.0f, -2.0f}, {0.0f, 0.0f}},
                {{ 1.0f, -1.0f, -2.0f}, {0.0f, 0.0f}},
                {{ 1.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
                {{-1.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 0.0f,  1.0f, -1.0f}, {0.0f, 0.0f}}
            };
            cube.vertex_count = 5;
            cube.index_array = (unsigned int[18]){1, 0, 4, 2, 1, 4, 3, 2, 4, 0, 3, 4, 0, 2, 3, 0, 1, 2};
            cube.index_count = 18;

            int sponza_file_size;
            byte *sponza_obj = read_entire_file(L"../assets/cube.obj", &sponza_file_size);
            assert(sponza_obj);
            struct scene scene = obj_parse(sponza_obj, sponza_file_size);
            free(sponza_obj);

            view_control_t control = {
                .position = {0.0f, 0.0f, 5.0f},
                .forward = {0.0f, 0.0f, -1.0f},
                .up = {0.0f, 1.0f, 0.0f},
                .fov = 0.18f,
                .speed = 3.0f,
                .sensitivity = 0.0003f,
            };

            LARGE_INTEGER fr, t1, t2 = {0};
            QueryPerformanceFrequency(&fr);

            int frame_count = 0;
            double delta_time = 0.0f, time_passed = 0.0f;
            ShowWindow(window, SW_SHOWNORMAL);

            while(global_running) {
                t1.QuadPart = t2.QuadPart;
                QueryPerformanceCounter(&t2);
                delta_time = (t2.QuadPart - t1.QuadPart) / (double)fr.QuadPart;

                process_pending_messages();

                handle_input(delta_time, &control);

                RECT client_rect;
                GetClientRect(window, &client_rect);
                int window_width = client_rect.right;
                int window_height = client_rect.bottom;

                opengl_draw_scene(opengl, window_width, window_height, &scene, &control);

                SwapBuffers(renderer_dc);

                if(time_passed >= 1.0f) {
                    wchar_t dt_string[16];
                    swprintf(dt_string, 16, L"%.2f FPS", 1.0f / (time_passed / frame_count));
                    SetWindowText(window, dt_string);
                    time_passed = 0.0f;
                    frame_count = 0;
                }
                time_passed += delta_time;
                frame_count += 1;
            }
        }
        else {
            MessageBox(NULL, L"Could not create the window.", L"Error", MB_OK|MB_ICONERROR);
        }
    }
    else {
        MessageBox(NULL, L"Could not register the window class.", L"Error", MB_OK|MB_ICONERROR);
    }

    ExitProcess(0);
}