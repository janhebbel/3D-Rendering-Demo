typedef struct
{
    vec3 position;
    vec3 forward;
    vec3 up;
    float fov;
    float speed;
    float sensitivity;
} 
view_control_t;

typedef struct
{
    double yoffset;
    int    updated;
}
scroll_update_t;

ivec2 global_initial_cursor_position;
scroll_update_t global_scroll_update;

void handle_input(double delta_time, view_control_t *control)
{
    // left mouse button pressed
    if(is_down(MOUSE_RBUTTON)) 
    {
        double xpos, ypos;
                
        POINT cursor_position;
        GetCursorPos(&cursor_position);
        xpos = (double)cursor_position.x;
        ypos = (double)cursor_position.y;
        
        float dx = (float)(xpos - global_initial_cursor_position[0]) * control->sensitivity;
        float dy = -(float)(ypos - global_initial_cursor_position[1]) * control->sensitivity;
        
        if(dx || dy)
        {
            SetCursorPos(global_initial_cursor_position[0], global_initial_cursor_position[1]);
        }
        
        static float yaw = -0.25f;
        static float pitch = 0.0f;

        yaw += dx;
        pitch += dy;
        
        if(pitch > 0.245f) pitch = 0.245f;
        else if(pitch < -0.245f) pitch = -0.245f;

        control->forward[0] = cglm_cos(yaw) * cglm_cos(pitch);
        control->forward[1] = cglm_sin(pitch);
        control->forward[2] = cglm_sin(yaw) * cglm_cos(pitch);
        cglm_vec3_normalize_this(control->forward);

        vec3 add = {0};
        
        if(is_down('W'))
        {
            cglm_vec3_add(add, control->forward, add);
        }
        if(is_down('A'))
        {
            vec3 tmp;
            cglm_vec3_cross(control->forward, control->up, tmp);
            cglm_vec3_normalize_this(tmp);
            cglm_vec3_sub(add, tmp, add);
        }
        if(is_down('S'))
        {
            cglm_vec3_sub(add, control->forward, add);
        }
        if(is_down('D'))
        {
            vec3 tmp;
            cglm_vec3_cross(control->forward, control->up, tmp);
            cglm_vec3_normalize_this(tmp);
            cglm_vec3_add(add, tmp, add);
        }

        cglm_vec3_scale_this(add, control->speed * (float)delta_time);
        cglm_vec3_add(control->position, add, control->position);
    }

    if(global_scroll_update.updated)
    {
        float dfov = (float)global_scroll_update.yoffset / 100.0f;
        float new_fov = control->fov + dfov;
        if(new_fov > 0.0f && new_fov < 0.4f)
        {
            control->fov = new_fov;
        }
        
        global_scroll_update.updated = 0;
    }

    update_key_was_down();
}
