#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in mat4 aModel;

out vec3 fColor;
uniform mat4 view;
void main()
{
    fColor = vec3(0.5,0.5,0.5);
    gl_Position = view * aModel * vec4(aPos, 1.0);
}
