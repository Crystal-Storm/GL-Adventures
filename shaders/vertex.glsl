#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 colors;

out vec3 vertexColor;

uniform float uOffsetHorizontal;
uniform float uOffsetVertical;

void main(){
    vertexColor = colors;
    gl_Position = vec4(position.x+uOffsetHorizontal, position.y+uOffsetVertical, position.z, 1.0f);
}