#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 colors;

out vec3 vertexColor;

uniform mat4 uTranslate;
uniform mat4 uPerspective;

void main(){
    vertexColor = colors;

    vec4 newPosition = uPerspective * uTranslate * vec4(position, 1.0f);

    gl_Position = newPosition;
}