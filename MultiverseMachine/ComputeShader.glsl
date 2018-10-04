#version 450 core 
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_compute_variable_group_size : enable
#extension GL_ARB_shader_storage_buffer_object : enable

layout( local_size_variable ) in; 

layout(rgba32f, binding = 0) uniform image2D out1;

//ivec2 st =		ivec2(gl_GlobalInvocationID.xy);
//ivec2 is =		imageSize(out1);
//vec4 current =	imageLoad(out1, st);


layout( std140, binding=0 ) buffer DATA
{
	vec4 pos[];
};


void main(){

	vec4 col = pos[gl_GlobalInvocationID.x];
	imageStore(out1,ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.x),col);
	//imageStore(out1,ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.x),vec4(1));
}


//vec4 circle(vec2 pos, float radius){
//	float c = distance(st, pos) / radius;
//	return exp(1-vec4(c))-exp(1)/exp(1);
//}
//
//vec4 adot(vec2 pos){
//	float c = distance(st, pos) + 1;
//	return exp(1-vec4(c))*exp(1);
//}

//vec4 fc = black;
//
//fc += circle(is/2+vec2(5,0),15)*red;
//fc += circle(is/2+vec2(-5,0),5)*green;
//fc += circle(is/2+vec2(0,5),5)*blue;
//fc += adot(is/2+vec2(0,-5));
//
//imageStore(out1, st, fc);


