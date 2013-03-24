uniform sampler2D snakeSkin;

void main(){
   gl_FragColor = texture2D(snakeSkin,gl_TexCoord[0].xy)	;
}
