#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 colors;

out vec3 vertexColor;

void main(){
    vertexColor = colors;
    gl_Position = vec4(position, 1.0f);
}