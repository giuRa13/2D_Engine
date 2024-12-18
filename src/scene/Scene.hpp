#pragma once

#include <entt/entt.hpp>
#include <TimeStep.hpp>


namespace ENGINE
{

	class Scene
	{
	public:
		Scene();
		~Scene();
		
		entt::entity CreateEntity();
		void OnUpdate(TimeStep ts);

		// TEMP
		entt::registry& Reg() { return m_Registry; }


	private:
		entt::registry m_Registry;
	};

}