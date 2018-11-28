#version 400 core

in vec3 fragNormal;
in vec2 TexCoord;
in vec3 fragPos;
in vec3 fragPosLocal;

out vec4 FragColor;

uniform sampler2D myTexture;
uniform sampler2D depthTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float time;

void main() {
	vec4 baseColor = vec4(0.0, 0.0, 1.0, 1.0);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPos);

	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColor;

	float diffuseStrenth = 0.8;
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStrenth * diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 100);
	vec3 specular = specularStrength * spec * lightColor;

	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (distance * distance);

	vec3 resultTextured = ambient * vec3(texture(myTexture, TexCoord))
							+ diffuse * vec3(texture(myTexture, TexCoord)) * attenuation 
							+ specular * attenuation;
	
	float Pole = pow((fragPosLocal.y + 1.0)/2, 2);
	float Rim = 1.0 - abs(dot(norm, viewDir));
	float Intersect = 0.0;

	vec4 _Color = vec4(16, 59, 159, 4) / 255.0;
    float glow = max(max(Intersect, Rim), Pole);
	vec4 glowColor = mix(_Color, vec4(1.0), glow);

	vec2 mainUV = vec2(TexCoord.x * 5, TexCoord.y * 9);
	vec4 mainTex = texture2D(myTexture, mainUV);
	float multiplier = sin(fragPosLocal.y*6 + time*4);
	vec4 pulsedTexColor  = multiplier * mainTex * _Color;

	vec4 outColor = pulsedTexColor + glowColor;

	//FragColor = vec4(fragNormal, 1.0f);
	//FragColor = vec4(Rim);
	FragColor = outColor;
	//FragColor = texture(depthTexture, gl_FragCoord.xy);
	//FragColor = vec4(resultTextured, 1.0);
}