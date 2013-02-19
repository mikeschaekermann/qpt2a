#version 110

uniform vec2 screenSize;
uniform float maxOpacity;
uniform int numOfCells;

uniform vec2 centers2D[169];
uniform float innerRadii2D[169];
uniform float outerRadii2D[169];

varying vec2 positionNormalized;

void main() {	
	vec2 screenSize1 = screenSize;
	vec2 screenPosition = (positionNormalized + vec2(1., 1.)) * 0.5 * screenSize1;

	float maxOpacity2 = 0.5;
	float opacity = maxOpacity;

	for (int i = 0; i < numOfCells; ++i)
	{
		float diffX = screenPosition.x - centers2D[i].x;
		float diffY = screenPosition.y - centers2D[i].y;

		float distanceSq = diffX * diffX + diffY * diffY;

		if (distanceSq < innerRadii2D[i])
		{
			opacity = 0.;
		}
		else if (distanceSq < outerRadii2D[i])
		{
			float opacityPercentage = (distanceSq - innerRadii2D[i]) / (outerRadii2D[i] - innerRadii2D[i]);
			float currentOpacity = opacityPercentage * maxOpacity;
			opacity = min(opacity, currentOpacity);
		}
	}

	gl_FragColor = vec4(0., 0., 0., opacity);
}