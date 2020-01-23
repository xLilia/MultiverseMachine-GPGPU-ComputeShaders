#version 450 core  

uniform sampler2D _LastFrame;

in vec2 tex_coord;
out vec4 _defaultOutput;

void main(){

	_defaultOutput = texture2D(_LastFrame,tex_coord); //CopyLastFrame

}