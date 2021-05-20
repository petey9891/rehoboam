// Adds the animation for the circle moving back and forth
// v1 - the coordinates of circle
// v2 - the boundtry in which the circle can move away from its coordinates?
// strength - the number of distortions on the circle
// speed - the speed of the animation
float variance(float normalizedCoord, float strength, float speed) {
	return sin(normalizedCoord * strength + time * speed) / 100.0;
}

// Creates circle float. Adds a variance animation to the circle.
// uv - the coordinates of the circle
// rad - the circle radius
// width - the circle's stroke
float circle(vec2 uv, float rad, float width) {
    float strength = 5.0;
    float speed = 2.0;

    float frame = length(uv);
    vec2 normalizedCoords = normalize(uv);
    // adds variance to each half of the circle by providing normalized x and y coords
    frame += variance(normalizedCoords.y, strength, speed) - variance(normalizedCoords.x, strength, speed);

    // Multiply by threadf to enlarge one portion of the circle
    float frameWidth = width + width*threadf*0.1;

    return smoothstep(rad-frameWidth, rad, frame) - smoothstep(rad, rad+frameWidth, frame);
}
