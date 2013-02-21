#version 110

uniform vec4 lightPos;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;

varying vec4 vertexPositionModel;
varying vec4 vertexPositionEye;
varying vec4 vertexNormalEye;

const vec3 viewPos = vec3(0., 0., 0.);

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void) {
	vec4 nLightVec = normalize(lightPos - vertexPositionEye);
	vec4 nVertexNormal = normalize(vertexNormalEye);
	float vertexLightAngle = dot(nLightVec, nVertexNormal);

	vec4 diffColor = diffuseColor * clamp(vertexLightAngle , 0., 1.);	

	gl_FragColor = ambientColor + diffColor * rand(vertexPositionModel.xy);
}



