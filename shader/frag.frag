#version 440

in vec3 Color;
//in vec2 TextCoord;
out vec4 FragColor;

//uniform sampler2D ourTexture;

void main()
{
    FragColor = vec4( Color, 1.0);
    //FragColor = texture2D(ourTexture, TextCoord);//对纹理指定位置进行采样
}
