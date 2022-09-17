#version 460 core

in vec2 fTexCoord;
in float lighting;
uniform sampler2D uTexture;

out vec4 Color;

void main() {
    Color = texture(uTexture, fTexCoord) * lighting;
}