#version 330
varying float height;
void main()
{
	gl_Position = (gl_ModelViewProjectionMatrix * gl_Vertex);
	// normalize height component
	height = (gl_Vertex.z + 1)/10;
} 
