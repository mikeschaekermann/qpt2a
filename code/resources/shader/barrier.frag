#version 110

uniform vec4 lightPos;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;

uniform float radius;
varying float currentRadius;

varying vec4 vertexPositionEye;
varying vec4 vertexNormalEye;

const vec3 viewPos = vec3(0., 0., 0.);

void main(void) {
	vec4 nLightVec = normalize(lightPos - vertexPositionEye);
	vec4 nVertexNormal = normalize(vertexNormalEye);
	float vertexLightAngle = dot(nLightVec, nVertexNormal);
	
	vec4 nLightReflectVec = normalize(reflect(nLightVec, nVertexNormal));
	vec4 nViewVec = normalize(vertexPositionEye - vec4(viewPos, 1.));
	float viewReflectAngle = dot(nLightReflectVec, nViewVec);
	
	vec4 diffColor = diffuseColor * clamp(vertexLightAngle , 0., 1.);

	gl_FragColor = ambientColor + diffColor * (currentRadius / radius);
}



