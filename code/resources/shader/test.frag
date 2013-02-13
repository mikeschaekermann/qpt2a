#version 110

uniform vec4 lightPos;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform float shininess;

varying vec4 vertexPositionEye;
varying vec4 vertexNormalEye;

const vec3 viewPos = vec3(0., 0., 0.);

float powing(float x, float y){
	float result = x;
	for(float i = 0.; i < y - 1.; ++i){
		result *= x;
	}

	return result;
}

void main(void) {
	vec4 nLightVec = normalize(lightPos - vertexPositionEye);
	vec4 nVertexNormal = normalize(vertexNormalEye);
	float vertexLightAngle = dot(nLightVec, nVertexNormal);
	
	vec4 nLightReflectVec = normalize(reflect(nLightVec, nVertexNormal));
	vec4 nViewVec = normalize(vertexPositionEye - vec4(viewPos, 1.));
	float viewReflectAngle = dot(nLightReflectVec, nViewVec);
	
	vec4 diffColor = vec4(diffuseColor * clamp(vertexLightAngle , 0., 1.), 1.);
	vec4 specColor = vec4(specularColor * powing(clamp(viewReflectAngle, 0., 1.), shininess), 1.);

	gl_FragColor = vec4(ambientColor, 1) + diffColor + specColor;
}



