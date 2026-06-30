#version 450 core
layout(location = 0) in vec3 pos_a;

uniform mat4 transformation;
void main(){
	gl_Position = transformation * vec4(pos_a,  1.0);
}
