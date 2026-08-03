#version 450 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
layout (location = 1) in mat4 aModel;

out vec3 fColor;
uniform mat4 view;
void main()
{
    fColor = vec3(1,1,1);
    gl_Position = view * aModel * vec4(aPos, 1.0);
}
