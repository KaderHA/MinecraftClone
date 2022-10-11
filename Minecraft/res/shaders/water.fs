#version 460 core

in vec2 fTexCoord;
in vec4 fClipSpace;

uniform sampler2D uTexture;
uniform sampler2D uReflectionTexture;
uniform sampler2D uRefractionTexture;

out vec4 Color;

void main() {
    vec2 ndc = (fClipSpace.xy / fClipSpace.w) / 2.0 + 0.5;
    vec2 reflectUV = vec2(ndc.x, -ndc.y);
    Color = mix(mix(texture(uReflectionTexture, reflectUV),texture(uRefractionTexture, ndc), 0.5), texture(uTexture, fTexCoord), 0.5);
    // Color = mix(texture(uTexture, fTexCoord), texture(uReflectionTexture, reflectUV), 0.5);
    // Color = mix(texture(uTexture, fTexCoord), texture(uRefractionTexture, ndc), 0.5);
}