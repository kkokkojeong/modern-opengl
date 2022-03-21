#version 330

in vec4 vCol;
in vec2 vTexCoord;

out vec4 colour;

uniform sampler2D uTex;

void main()
{
    colour = texture(uTex, vTexCoord);
//	colour = texture(uTex, vTexCoord) * vCol;
}
