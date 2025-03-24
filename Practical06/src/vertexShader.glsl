#version 330 core

layout(location = 0) in vec3 sv_position; // Vertex position
layout(location = 1) in vec3 sv_color;    // Vertex color

out vec3 color; // Pass color to the fragment shader
uniform mat4 model; // Rotation matrix

void main()
{
    color = sv_color;
    gl_Position = model * vec4(sv_position, 1.0);
}