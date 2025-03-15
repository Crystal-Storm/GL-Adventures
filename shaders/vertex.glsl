#version 410 core
in vec4 position;
out vec3 vertexColor;
void main(){
    vertexColor = vec3(.5,.5,.5);
    gl_Position = vec4(position.x, position.y, position.z, position.w);
}