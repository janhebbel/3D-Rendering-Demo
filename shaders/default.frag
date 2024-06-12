#version 430 core

in vec2 uv;
                                      
//layout(location = 1) uniform sampler2D u_texture;

layout(location = 0) out vec4 frag_color;

void main() 
{
    frag_color = vec4(0.14, 0.35, 0.22, 1.0); // texture(u_texture, uv)
}
