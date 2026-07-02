#version 450 core
layout(location = 0) in vec3 pos_a;

uniform mat4 view;
uniform mat4 model;
uniform mat4 world;
void main(){
	gl_Position = world*view *model* vec4(pos_a,  1.0);
}
