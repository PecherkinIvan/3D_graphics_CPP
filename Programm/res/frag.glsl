#version 330
#extension GL_ARB_separate_shader_objects : enable


in vec3 position;
out vec4 a_color;

void main(){
	a_color = vec4(0, 1, 0, 1);
	gl_Position = vec4(position, 1);
}