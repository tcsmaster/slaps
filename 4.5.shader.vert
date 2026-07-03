#version 450 core
layout(location = 0) in vec3 pos_a;
layout(location = 1) in vec3 normal_a;
out vec3 fragpos;
out vec3 normal;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
void main(){
	fragpos = vec3(model * vec4(pos_a, 1.f));
	normal = normal_a;
	gl_Position = projection*view * vec4(fragpos, 1.0);
}
