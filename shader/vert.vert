#version 440

layout (location = 0) in vec3 VertexPosition;//顶点坐标
//layout (location = 2) in vec2 tempTextCoord;//纹理坐标

//三个坐标变化矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec2 TextCoord;
out vec3 Color;

void main()
{
    gl_Position = projection * view * model * vec4( VertexPosition, 1.0 );
    Color = vec3(1.0, 0.0, 0.0);
    //TextCoord = tempTextCoord;
}
