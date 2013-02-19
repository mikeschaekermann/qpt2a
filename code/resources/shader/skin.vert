#version 110

uniform float time;

varying vec4 vertexPositionEye;
varying vec4 vertexNormalEye;

void main()
{
	vec4 displacement = vec4(cos(gl_Normal * time / 3.), 0.);
	vertexPositionEye = gl_ModelViewMatrix * (gl_Vertex + displacement);
	vertexNormalEye = normalize(vec4(gl_NormalMatrix * (gl_Normal + vec3(displacement)), 0.));

	gl_Position = gl_ProjectionMatrix * vertexPositionEye;
}
