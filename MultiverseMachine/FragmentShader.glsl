#version 450 core  
precision mediump float;

uniform sampler2D _LastFrame;
uniform vec2 _oneDivPxpu;
uniform unsigned int _t;

in vec2 tex_coord;


vec4 get(vec2 Offset){
	return texture2D(_LastFrame,tex_coord + Offset*1/100);
}

vec4 getT(vec2 Offset, sampler2D TexC){
	return texture2D(TexC,tex_coord + Offset*_oneDivPxpu);
}

vec4 current =	get(vec2(0,0));
vec4 L =		get(vec2(-1,0));
vec4 R =		get(vec2(+1,0));
vec4 U =		get(vec2(0,+1));
vec4 D =		get(vec2(0,-1));
vec4 UL =		get(vec2(-1,+1));
vec4 UR =		get(vec2(+1,+1));
vec4 DL =		get(vec2(-1,-1));
vec4 DR =		get(vec2(+1,-1));
vec4 sum =		L+R+U+D+UL+UR+DL+DR;

vec4 st	=		vec4(tex_coord, 0, 0);
vec4 white =	vec4(1,1,1,0);
vec4 black =	vec4(0);
vec4 red =		vec4(1,0,0,1);
vec4 green =	vec4(0,1,0,1);
vec4 blue =		vec4(0,0,1,1);

//CUSTOM

uniform sampler2D in1;
out vec4 out1;
out vec4 _defaultOutput;

void main(){
	
	//Rules

	_defaultOutput = getT(vec2(0,0),in1); //current//vec4(sin(_t));


	//if(float(current) != 0){
	//	_defaultOutput = current;
	//}

	//if(float(sum) == 0){
	//	_defaultOutput /= 8;
	//}
	//
	//if(float(sum) > 0){
	//	_defaultOutput = sum/8;
	//}

	//if (current == white){
	//	out1 = white;
	//}

	//if(current.r == red.r){
	//	_defaultOutput = red;
	//}

	




	
	
}

	//_outColor = vec4(fract(sin(_t)));
	//_outColor = vec4(0,0,1,1);

	//if(L  == red 
	//|| R  == red 
	//|| U  == red 
	//|| D  == red
	//|| UL == red
	//|| UR == red
	//|| DL == red
	//|| DR == red)
	//{
	//	 _outColor = green;
	//}