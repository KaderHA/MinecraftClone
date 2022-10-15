#version 460 core

in vec2 fTexCoord;
in vec4 fClipSpace;
in vec3 fToCamera;

uniform sampler2D uTexture;
uniform sampler2D uReflectionTexture;
uniform sampler2D uRefractionTexture;

out vec4 Color;

void main() {
    vec2 ndc = (fClipSpace.xy / fClipSpace.w) / 2.0 + 0.5;
    vec3 viewVector = normalize(fToCamera);
    float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));

    Color = mix(mix(texture(uReflectionTexture,  vec2(ndc.x, -ndc.y)), texture(uRefractionTexture, ndc), refractiveFactor), texture(uTexture, fTexCoord), 0.5);
    // Color = mix(texture(uReflectionTexture,  vec2(ndc.x, -ndc.y)), texture(uRefractionTexture, ndc), refractiveFactor);
    // Color = mix(texture(uTexture, fTexCoord), texture(uRefractionTexture, ndc), 0.5);
}