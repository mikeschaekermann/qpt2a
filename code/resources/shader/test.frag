#version 110

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform float shininess;

varying vec4 vertexPositionEye;
varying vec4 vertexNormalEye;

//varying vec4 outColor;

float clamp(float x, float minValue, float maxValue)
{
	return min(max(x, minValue), maxValue);
}

void main(void) {
	vec4 nLightVec = normalize(vec4(lightPos, 1) - vertexPositionEye);
	vec4 nVertexNormal = normalize(vertexNormalEye);
	float vertexLightAngle = dot(nLightVec, nVertexNormal);
	
	vec4 nLightReflectVec = normalize(reflect(nLightVec, nVertexNormal));
	vec4 nViewVec = normalize(vertexPositionEye - vec4(viewPos, 1));
	float viewReflectAngle = dot(nLightReflectVec, nViewVec);
	
	vec4 diffColor = vec4(diffuseColor * clamp(vertexLightAngle , 0., 1.), 1);
	vec4 specColor = vec4(specularColor * pow(clamp(viewReflectAngle, 0., 1.), shininess), 1);

	gl_FragColor = vec4(ambientColor, 1) + diffColor + specColor;

	// the only way the model is rendered
	//gl_FragColor = outColor = vec4(1,1,1,1);
}