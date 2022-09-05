#version 450 core
out vec4 Color;

in vec3 fTexCoord;

uniform samplerCube uSkybox;

void main()
{    
    Color = texture(uSkybox, fTexCoord);
}