//Fragment shader for panel
uniform sampler2D tex;
const vec3 black = vec3(0,0,0);
const vec3 background = vec3(0.8,0.8,1);
void main(){
	vec2 at = gl_TexCoord[0].st;
	vec4 color = texture2D(tex,at);
	if (color.rgb == black) gl_FragColor = vec4(background,0.8);
	else gl_FragColor = color;
}
