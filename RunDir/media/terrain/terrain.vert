#version 140

in vec3 position;

varying float height;
void main()
{
	vec4 vertex = vec4(position.xyz,1);
	gl_Position = (gl_ModelViewProjectionMatrix * vertex);
	// normalize height component
	height = (position.z + 1)/10;
} 
