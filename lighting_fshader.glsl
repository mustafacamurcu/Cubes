#version 330 core

uniform vec3 objectColor;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 cameraLightColor;
uniform vec3 cameraPos;
uniform bool cameraLightOn;

uniform bool specLightOn;

in vec3 Normal; 
in vec3 FragPos;

out vec4 FragColor;

void main() {
	float ambientStrength = 0.2;
	float specularStrength = 0.5;

	vec3 total_light = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	total_light = total_light + diffuse;

	vec3 viewDir = normalize(cameraPos - FragPos);

	if (specLightOn) {
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
		vec3 specular = specularStrength * spec * lightColor;
		total_light = total_light + specular;
	}
	

	if (cameraLightOn) {
		float cameraDiff = max(dot(norm, viewDir), 0.0);
		vec3 cameraDiffuse = cameraDiff * cameraLightColor;
		total_light = total_light + cameraDiffuse;
	}


	total_light = total_light * objectColor;
	FragColor = vec4(total_light, 1.0);
}
