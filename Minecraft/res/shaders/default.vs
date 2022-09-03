#version 460 core

layout(location = 0) in vec3 aPosition;
// layout(location = 1) in vec2 aUV;
// layout(location = 2) in vec3 aNormal;

uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uView * vec4(aPosition, 1.0);
}
