// grayscale.fs
//
// convert RGB to grayscale

void main(void)
{
    // Convert to grayscale using NTSC conversion weights
    float gray = dot(gl_Color.rgb, vec3(0.299, 0.587, 0.114));
	//float gray = dot(gl_Color.rgb, vec3(0, 1, 1));

    // replicate grayscale to RGB components
    gl_FragColor = vec4(gray, gray, gray, 1.0);
	//gl_FragColor = vec4(1, 1, 0, 1.0);
}
