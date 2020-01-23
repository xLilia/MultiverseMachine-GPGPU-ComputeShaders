#include "EntityManager.h"

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{
	CompactEntityBUFFER.empty();
	delete this->EntitySSBO;
}

void EntityManager::addEntity(Entity newEntity)
{
	CompactEntityBUFFER.push_back(MakeCompactEntity(newEntity));
}

EntityManager::Entity EntityManager::getEntity(GLuint Index)
{
	auto it = CompactEntityBUFFER.begin();
	std::advance(it, Index);
	CompactEntity cE = *it;
	return MakeEntity(cE.PosVel2D.x, cE.PosVel2D.y, cE.PosVel2D.z, cE.PosVel2D.w, cE.RGBMass.r, cE.RGBMass.g, cE.RGBMass.b, cE.RGBMass.w);
}

void EntityManager::setEntity(GLuint Index, Entity E)
{
	auto it = CompactEntityBUFFER.begin();
	std::advance(it, Index);
	CompactEntityBUFFER.emplace(it, MakeCompactEntity(E));
	CompactEntityBUFFER.erase(it);
	
}

RenderAux::ComputeBlock* EntityManager::MakeEntitySSBO()
{
	std::vector<CompactEntity>ceb;
	ceb.assign(CompactEntityBUFFER.begin(), CompactEntityBUFFER.end());
	EntitySSBO = RenderAux::CreateComputeSSBO(ceb.size(), &ceb[0]);
	return EntitySSBO;
}
