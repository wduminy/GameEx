uniform float left;
uniform float top;
uniform float width;
uniform float height;
void main()
{
	gl_Position.x = gl_Vertex.x ;
	gl_Position.y = gl_Vertex.y ;
/*   gl_Position = (gl_ModelViewProjectionMatrix * gl_Vertex); */
   gl_TexCoord[0].x = (gl_Vertex.x - left) / width;
   gl_TexCoord[0].y = (-gl_Vertex.y + top) / height;
} 