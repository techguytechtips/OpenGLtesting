#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D tex0;
uniform vec4 lightColor;
void main()
{
FragColor = texture(tex0, TexCoord) * lightColor;
}
