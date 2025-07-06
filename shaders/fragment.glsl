#version 330 core

in vec3 our_color;
in vec2 tex_coord;

uniform sampler2D container_texture;
uniform sampler2D face_texture;
uniform float face_alpha;

out vec4 frag_color;

void main()
{
    frag_color = mix(texture(container_texture, tex_coord), texture(face_texture, vec2(tex_coord.x, tex_coord.y)), 0.2);
};