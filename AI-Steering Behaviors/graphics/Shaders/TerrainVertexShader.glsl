#version 330                                                                       
                                                                                    
layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 UVs;                                             
layout (location = 2) in vec3 Normal; 

out vec2 textCoord;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

uniform mat4 camera;
uniform mat4 transformationMatrix,projectionMatrix,viewMatrix;
uniform vec3 lightPos;
uniform float scaleUV; 


void main()
{
	textCoord = UVs * scaleUV;
	vec4 worldPosition =  transformationMatrix *  vec4(Position,1.0f);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;

	
	surfaceNormal  = normalize(transpose(inverse(transformationMatrix)) * vec4(Normal, 0.0f)).xyz;
	toLightVector =  normalize(lightPos -  worldPosition.xyz);

	vec3 viewPos = (inverse(viewMatrix) * vec4(0.0f,0.0f,0.0f,1.0f)).xyz;

	toCameraVector =  normalize(viewPos -  worldPosition.xyz);

}