#version 330
#extension GL_ARB_separate_shader_objects : enable

in vec4 a_color;
out vec4 f_color;

void main(){
	f_color = a_color;
}