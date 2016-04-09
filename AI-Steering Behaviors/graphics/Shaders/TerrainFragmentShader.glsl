#version 410 

struct Light {
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


out vec4 out_Color;

uniform bool isTextured;
uniform bool hasDiffusMap;
uniform bool hasSpecularMap;

in vec2 textCoord;


in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform Light light; 

uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 ambient;
uniform float shininess;



void main()
{
	vec3 Kd = diffuse.rgb;	
	vec3 Ks  = specular.rgb;
	vec3 Ka = ambient.rgb;

	if(isTextured == true)
	{
	  
		if(hasDiffusMap == true)
		{
			
		    Kd = texture2D(diffuseMap,textCoord).rgb;
			Ka = texture2D(diffuseMap,textCoord).rgb;
			
		}
		if(hasSpecularMap == true)
		{
			Ks = texture2D(specularMap,textCoord).rgb;
		}
		
	}

	
	vec3 Ia = light.ambient * Ka;

	float brightness = max(dot(surfaceNormal,toLightVector),0);
	vec3 Id = light.diffuse * (brightness * Kd);



	vec3 reflectDir = reflect(-toLightVector, surfaceNormal);  
	float dotProductSpecular =  max(dot(toCameraVector,reflectDir),0);
	float specularFactor = pow(dotProductSpecular,10);


	vec3 Is = light.specular * (specularFactor * Ks);

	
	
	//out_Color = vec4(Ia + Id + Is, 1.0);
	out_Color = texture2D(diffuseMap,textCoord);
}
