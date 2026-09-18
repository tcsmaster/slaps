#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in mat4 aModel;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 view;

void main()
{
    FragPos = vec3(aModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(aModel))) * aNormal;  
    gl_Position =  view * vec4(FragPos, 1.0);
}
