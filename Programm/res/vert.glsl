#version 330
#extension GL_ARB_separate_shader_objects : enable

in vec4 a_color;
in vec2 a_texCoord;
out vec4 f_color;

uniform sampler2D u_texture0;

void main(){
	f_color = a_color * texture(u_texture0, a_texCoord);
}
