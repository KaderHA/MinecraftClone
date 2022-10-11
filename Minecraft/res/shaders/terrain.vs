#version 460 core

layout(location = 0) in uint aData;

uniform samplerBuffer uTexUV;
uniform mat4 uViewProjection;
uniform mat4 uModel;
uniform vec4 uPlane;


out vec2 fTexCoord;
out float lighting;
vec2 GetTexCoord();

void main() {
    vec4 worldPosition = uModel * uvec4((aData & 0x3F), ((aData >> 6) & 0x3F), ((aData >> 12) & 0x3F), 1.0);

    gl_ClipDistance[0] = dot(worldPosition, uPlane);

    gl_Position = uViewProjection * worldPosition;
    fTexCoord = GetTexCoord();
    uint AOLight = ((aData >> 20) & 3);
    if (AOLight == 0) lighting = 1.0;
    else if (AOLight == 1) lighting = 0.8;
    else if (AOLight == 2) lighting = 0.6;
    else if (AOLight == 3) lighting = 0.4;
}

vec2 GetTexCoord() {
    uint vertex = ((aData >> 18) & 3);
    int texIndex = int(((aData >> 22) * 8) + (vertex * 2));

    return vec2(texelFetch(uTexUV, texIndex + 0).r, texelFetch(uTexUV, texIndex + 1).r);
}