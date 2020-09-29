	#version 330
	in vec4 vPosition;
	in vec4 vColor;
	out vec4 color;
	uniform  mat4 scale;
	uniform  mat4 rotationX;
	uniform  mat4 rotationY;
	uniform  mat4 rotationZ;
	void main()
	{
		color = vColor;
		gl_Position = rotationX * rotationY * rotationZ * scale * vPosition;
	}