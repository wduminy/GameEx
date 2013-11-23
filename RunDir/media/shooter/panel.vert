uniform float left;
uniform float top;
uniform float width;
uniform float height;
void main() {
	gl_Position.xy = gl_Vertex.xy;
	gl_TexCoord[0].x = (gl_Vertex.x - left) / width;
	gl_TexCoord[0].y = -(gl_Vertex.y + top) / height;
} 