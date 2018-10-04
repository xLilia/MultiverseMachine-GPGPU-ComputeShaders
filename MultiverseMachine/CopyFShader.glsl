#version 450 core  

uniform sampler2D _copyStep;

in vec2 tex_coord;
out vec4 _outColor;

void main(){

	_outColor = texture2D(_copyStep,tex_coord);

}