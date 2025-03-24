#version 330 core

layout(location = 0) in vec3 sv_position;  // Vertex position
layout(location = 1) in vec3 sv_color;     // Vertex color

out vec3 color;  // Color to pass to fragment shader

uniform mat4 model;  // Model transformation matrix

void main() {
    gl_Position = model * vec4(sv_position, 1.0);
    color = sv_color;
}
