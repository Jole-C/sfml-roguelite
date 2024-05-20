#version 120

uniform sampler2D texture;
uniform float threshold;

void main(void)
{

	vec3 currentColour = texture2D(texture, gl_TexCoord[0].xy).rgb;

	vec4 pixel = vec4(currentColour.rgb, 0.0);

	float brightness = dot(currentColour.rgb, vec3(0.2126, 0.7152, 0.0722));

	if(brightness >= threshold)
	{
		pixel = texture2D(texture, gl_TexCoord[0].xy);
	}

}