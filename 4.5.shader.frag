#version 450 core
layout(location = 0) out vec4 frag_color;
in vec3 normal;
in vec3 fragpos;
uniform vec3 viewpos;
uniform vec3 lightcolor;
uniform vec3 objectcolor;
uniform vec3 lightpos;
void main(){

	float consta = 0.1;
	vec3 ambient_color = consta * lightcolor;

	vec3 norm = normalize(normal);
	vec3 diff = normalize(lightpos - fragpos);
	float diffa = max(dot(norm, diff), 0.0);
	vec3 diffuse = diffa*lightcolor;

	float specularStrength = 0.5;
	vec3 viewdir = normalize(viewpos - fragpos);
	vec3 reflectdir = reflect(-diff, norm);
	float spec = pow(max(dot(viewdir, reflectdir), 0.0), 32);
	vec3 specular = spec*specularStrength*lightpos;
	vec3 final_color = (ambient_color + diffuse + specular) * objectcolor;
	frag_color = vec4(final_color, 1.0);
}
