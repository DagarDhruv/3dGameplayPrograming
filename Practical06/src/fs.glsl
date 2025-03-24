#version 330 core

in vec3 color;  // Color passed from vertex shader
out vec4 FragColor;  // Final color of the fragment

void main() {
    FragColor = vec4(color, 1.0f);
}
