#version 410 

uniform sampler2D diffuseMap;
out vec4 out_Color;

in vec2 textCoord;

void main()
{

	out_Color = texture2D(diffuseMap,textCoord);
}

