#pragma once
#include <list>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "RenderAux.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	struct Entity {
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec3 color;
		float mass;
	};

	struct CompactEntity {
		glm::vec4 PosVel2D;
		glm::vec4 RGBMass;
	};

	static Entity MakeEntity(float x, float y, float vx, float vy, float m, float r, float g, float b) {
		Entity E{ glm::vec2(x,y),glm::vec2(vx,vy),glm::vec3(r,g,b),m };
		return E;
	}

	static CompactEntity MakeCompactEntity(Entity E) {
		CompactEntity CE = { glm::vec4(E.position.x, E.position.y, E.velocity.x, E.velocity.y), glm::vec4(E.color.r,E.color.g,E.color.b,E.mass) };
		return CE;
	}

	void printEntity(GLuint Index) {
		printEntity(getEntity(Index));
	}

	static void printEntity(Entity E) {
		std::cout 
			<< E.position.x << "_"
			<< E.position.y << "_"
			<< E.velocity.x << "_"
			<< E.velocity.y << "_"
			<< E.color.r << "_"
			<< E.color.g << "_"
			<< E.color.b << "_"
			<< E.mass << " ";
	}

	//static std::vector<float> getCompactEntity(EntityManager::Entity E)
	//{
	//	std::vector<float> Compact;
	//	//PosVel2D
	//	Compact.push_back(E.position.x);
	//	Compact.push_back(E.position.y);
	//	Compact.push_back(E.velocity.x);
	//	Compact.push_back(E.velocity.y);
	//	//ColorMass
	//	Compact.push_back(E.color.r);
	//	Compact.push_back(E.color.g);
	//	Compact.push_back(E.color.b);
	//	Compact.push_back(E.mass);
	//	return Compact;
	//}

	void addEntity(EntityManager::Entity newEntity);
	EntityManager::Entity getEntity(GLuint Index);
	void setEntity(GLuint Index, Entity E);
	RenderAux::ComputeBlock* MakeEntitySSBO();
	RenderAux::ComputeBlock* EntitySSBO;
private:
	std::list<CompactEntity> CompactEntityBUFFER;
	std::list<CompactEntity>::iterator EntityCursor;
};

