#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

void main()
{
	// Set solid color(blue).
	color = vec4(0.f, 0.02, 0.9, 1.f);
}