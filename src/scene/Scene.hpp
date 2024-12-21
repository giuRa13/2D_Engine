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
		void DestroyEntity(Entity entity);

		void OnUpdate(TimeStep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();


	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}