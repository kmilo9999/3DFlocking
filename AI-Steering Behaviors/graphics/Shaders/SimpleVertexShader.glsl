#version 330                                                                       
                                                                                    
layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 UVs;                                             


out vec2 textCoord;

uniform mat4 camera;
uniform mat4 transformationMatrix,projectionMatrix,viewMatrix;


void main()
{
	textCoord = UVs;
	vec4 worldPosition =  transformationMatrix *  vec4(Position,1.0f);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;

}