#version 450 core
layout(location = 0) out vec4 frag_color;
in vec3 normal;
in vec3 fragpos;
uniform vec3 viewpos;
struct Light{
	vec3 position;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};
uniform Light light;
struct Material{
	float shininess;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};
uniform Material material;
void main(){
	vec3 ambient_color = material.ambient * light.ambient;

	vec3 norm = normalize(normal);
	vec3 diff = normalize(light.position - fragpos);
	float diffa = max(dot(norm, diff), 0.0);
	vec3 diffuse = light.diffuse*diffa*material.diffuse;

	vec3 viewdir = normalize(viewpos - fragpos);
	vec3 reflectdir = reflect(-diff, norm);
	float spec = pow(max(dot(viewdir, reflectdir), 0.0), material.shininess);
	vec3 specular = spec*material.specular*light.specular;


	vec3 final_color = ambient_color + diffuse + specular;
	frag_color = vec4(final_color, 1.0);
}
