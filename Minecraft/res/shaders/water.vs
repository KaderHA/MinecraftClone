#version 460 core

layout(location = 0) in uint aData;

uniform samplerBuffer uTexUV;
uniform mat4 uViewProjection;
uniform mat4 uModel;
uniform vec3 uCameraPosition;

out vec2 fTexCoord;
out vec4 fClipSpace;
out vec3 fToCamera;
vec2 GetTexCoord();

void main() {
    vec4 worldPosition = uModel * vec4((aData & 0x3F), ((aData >> 6) & 0x3F), ((aData >> 12) & 0x3F), 1.0);
    worldPosition.y -= 0.1;
    fClipSpace = uViewProjection * worldPosition;
    gl_Position = fClipSpace;
    fToCamera = uCameraPosition - worldPosition.xyz;
    fTexCoord = GetTexCoord();
}

vec2 GetTexCoord() {
    uint vertex = ((aData >> 18) & 3);
    int texIndex = int(((aData >> 22) * 8) + (vertex * 2));

    return vec2(texelFetch(uTexUV, texIndex + 0).r, texelFetch(uTexUV, texIndex + 1).r);
}