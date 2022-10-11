#version 460 core

layout(location = 0) in uint aData;

uniform samplerBuffer uTexUV;
uniform mat4 uViewProjection;
uniform mat4 uModel;

out vec2 fTexCoord;
vec2 GetTexCoord();

void main() {
    uvec3 pos = uvec3((aData & 0x3F), ((aData >> 6) & 0x3F), ((aData >> 12) & 0x3F));

    gl_ClipDistance[0] = -1;

    gl_Position = uViewProjection * uModel * uvec4(pos, 1.0);
    fTexCoord = GetTexCoord();
}

vec2 GetTexCoord() {
    uint vertex = ((aData >> 18) & 3);
    int texIndex = int(((aData >> 20) * 8) + (vertex * 2));

    return vec2(texelFetch(uTexUV, texIndex + 0).r, texelFetch(uTexUV, texIndex + 1).r);
}