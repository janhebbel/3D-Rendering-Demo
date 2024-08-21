struct textured_vertex {
    vec4 position;
    vec2 uv;
    vec3 normal;
};

struct scene {
    struct textured_vertex *vertex_array;
    int vertex_count;

    unsigned int *index_array;
    int index_count;
};
