#pragma once

#include <entt/entt.hpp>
#include <TimeStep.hpp>


namespace ENGINE
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();
		
		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(TimeStep ts);

		// TEMP
		//entt::registry& Reg() { return m_Registry; }


	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}