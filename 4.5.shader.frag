#version 450 core
layout(location = 0) out vec4 frag_color;

uniform vec3 lightcolor;
uniform vec3 objectcolor;
void main(){
	frag_color = vec4(lightcolor*objectcolor, 1.f);
}
