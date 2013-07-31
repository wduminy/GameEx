#version 330
const vec3 white = vec3(1,1,1);
const vec3 brown = vec3(1,1,0);
const vec3 green = vec3(0,1,0);
const vec3 blue = vec3(0,0,1);
const float rock = 6;
const float grass = 3;
const float sand = -2;
const float water = -6;
void main()
{
	vec4 pos;
	pos.xyz = gl_Vertex.xyz;
	pos.w = 1.0f;
	gl_Position = (gl_ModelViewProjectionMatrix * pos);
	float h = pos.z;
	if (h < water) gl_FrontColor.xyz = blue;
	else if (h < sand) gl_FrontColor.xyz = brown;
	else if (h < grass) gl_FrontColor.xyz = green;
	else gl_FrontColor.xyz = white;  
} 
