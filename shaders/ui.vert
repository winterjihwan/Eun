#version 410 core

layout(location = 0) in vec4 aVertex;

out vec2 vTexCoords;

uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * vec4(aVertex.xy, 0.0, 1.0);
    vTexCoords = aVertex.zw;
}
