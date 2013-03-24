void main()
{
   gl_Position = (gl_ModelViewProjectionMatrix * gl_Vertex);
   gl_TexCoord[0].x = gl_Vertex.x;
   gl_TexCoord[0].y = -gl_Vertex.y;
} 