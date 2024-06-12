typedef struct
{
    vec3 position;
    vec2 uv;
}
textured_vertex_t;

typedef struct
{
    textured_vertex_t *vertex_array;
    int vertex_count;

    unsigned int *index_array;
    int index_count;
}
scene_t;
