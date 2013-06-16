
void main()
{
   gl_Position = (gl_ModelViewProjectionMatrix * gl_Vertex);
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_TexCoord[0].x /= 3.5;	
} 
