#version 110

uniform vec2 screenSize;

uniform int numOfCells;

uniform vec2 centers2D[];
uniform vec2 radii2D[];

varying vec2 positionNormalized;

float distance1(vec2 a, vec2 b)
{
	float diffX = a.x - b.x;
	float diffY = a.y - b.y;

	return sqrt(diffX * diffX + diffY * diffY);
}

void main() {	
	vec2 screenSize1 = vec2(1024., 768.);
	vec2 screenPosition = (positionNormalized + vec2(1., 1.)) * 0.5 * screenSize1;
	
	gl_FragColor = vec4(0., 0., 0., 0.);
}