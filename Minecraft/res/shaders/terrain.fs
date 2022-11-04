#version 460 core

in vec2 fTexCoord;
in float lighting;
uniform sampler2D uTexture;

out vec4 Color;

void main() {
    vec4 textureColor = texture(uTexture, fTexCoord);
    if(textureColor.a < 0.1)
        discard;
    Color = textureColor * lighting;
}