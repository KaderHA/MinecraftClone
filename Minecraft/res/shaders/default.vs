#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord; 
uniform mat4 uViewProjection;
uniform mat4 uModel;

out vec2 fTexCoord;

void main() {
    gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0);
    fTexCoord = aTexCoord;
}