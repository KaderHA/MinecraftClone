#version 450 core
layout(location = 0) in vec3 aPosition;

out vec3 fTexCoord;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    fTexCoord = aPosition;
    gl_Position = uProjection * uView * vec4(aPosition, 1.0);
}  