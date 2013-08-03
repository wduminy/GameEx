#version 330

const vec3 white = vec3(1,1,1);
const vec3 grey = vec3(0.75,0.75,0.75);
const vec3 green = vec3(0,1,0);
const vec3 blue = vec3(0,0,1);
const vec3 brown = vec3(0.7,0.6,0.1);
const float ice = 0.94;
const float rock = 0.65;
const float grass = 0.35;
const float sand = 0.06;
const float m = 8.5;   
const float u = 1.6;


varying float height;

float degree(float v) {
	return clamp(u - abs(height-v)*m,0,1);
}

void main(){
	float ice_d = degree(ice);
	float rock_d = degree(rock);
	float grass_d = degree(grass);
	float sand_d = degree(sand);
	float total = ice_d + rock_d + grass_d + sand_d;
    gl_FragColor = vec4(
    	ice_d * white / total + 
    	rock_d * grey / total + 
    	grass_d * green / total +  
    	sand_d * brown / total
    	, 1);
}
