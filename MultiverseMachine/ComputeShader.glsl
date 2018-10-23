#version 450 core 
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_compute_variable_group_size : enable
#extension GL_ARB_shader_storage_buffer_object : enable

layout( std140, binding=0 ) buffer DATA1
{
	vec4 col1[];
};


layout( std140, binding=1 ) buffer DATA2
{
	vec4 col2[];
};

 
layout(rgba32f, binding = 0) uniform image2D out1;

layout( local_size_variable ) in;

uint gidx = gl_GlobalInvocationID.x;
uint gidy = gl_GlobalInvocationID.y;

void main(){

	uint cord = (gidy*100)+gidx;
	vec4 color = col1[cord] + col2[cord];
	col2[cord]+=0.01;
	imageStore(out1,ivec2(gidx,gidy),color);

}

