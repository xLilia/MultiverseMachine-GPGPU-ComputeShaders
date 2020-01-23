#version 450 core 
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_compute_variable_group_size : enable
#extension GL_ARB_shader_storage_buffer_object : enable

layout( local_size_variable ) in;
uint gidx = gl_GlobalInvocationID.x;
uint gidy = gl_GlobalInvocationID.y;

//CUSTOM

struct Entity {
	vec4 PosVel2D;
	vec4 RGBMass;
};

layout( std140 , binding=0 ) buffer EntityData
{
	Entity CurrentEntity[];
};

layout(rgba32f) uniform image2D out_ParticlesTex;

void main(){

	uint cord = (gidy*100)+gidx;

	//CUSTOM
	vec2 Ppos = CurrentEntity[cord].PosVel2D.xy + CurrentEntity[cord].PosVel2D.zw;
	if(Ppos.x > 1920) CurrentEntity[cord].PosVel2D.z *= -1;
	if(Ppos.x < 0) CurrentEntity[cord].PosVel2D.z *= -1;
	if(Ppos.y < 0) CurrentEntity[cord].PosVel2D.w *= -1;
	if(Ppos.y > 1080) CurrentEntity[cord].PosVel2D.w *= -1;
	CurrentEntity[cord].PosVel2D = vec4(Ppos,CurrentEntity[cord].PosVel2D.zw);
	imageStore(out_ParticlesTex,ivec2(Ppos),CurrentEntity[cord].RGBMass);

}

