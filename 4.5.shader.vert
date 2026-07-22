#version 450 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
layout (location = 1) in mat4 aModel;

out vec3 fColor;

void main()
{
    fColor = vec3(0.5,0.5,0.5);
    gl_Position = aModel * vec4(aPos, 1.0);
}
