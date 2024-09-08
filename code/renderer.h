typedef struct Textured_Vertex {
    vec4 position;
    vec2 uv;
    vec3 normal;
} Textured_Vertex;

typedef struct Scene {
    Textured_Vertex *vertex_array;
    int vertex_count;

    unsigned int *index_array;
    int index_count;
} Scene;
