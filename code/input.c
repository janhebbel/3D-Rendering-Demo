enum {
    MOUSE_LBUTTON = 256,
    MOUSE_RBUTTON,
    MOUSE_MBUTTON,

    INPUT_KEY_COUNT
};

int key_down[INPUT_KEY_COUNT];
int key_was_down[INPUT_KEY_COUNT];

void key_event(int key, int down) {
    key_was_down[key] = key_down[key];
    key_down[key] = down;
}

void update_key_was_down(void) {
    for(int i = 0; i < INPUT_KEY_COUNT; ++i) {
        key_event(i, key_down[i]);
    }
}

void clear_keyboard_state(void) {
    for (int i = 0; i < INPUT_KEY_COUNT; ++i) {
        key_down[i] = 0;
        key_was_down[i] = 0;
    }
}

int is_down(int key) {
    return(key_down[key]);
}

int is_down_once(int key) {
    int result = (key_down[key] && !key_was_down[key]);
    return(result);
}
