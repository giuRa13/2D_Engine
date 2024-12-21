#pragma once

#include <Base.hpp>
#include <Log.hpp>
#include <scene/Scene.hpp>
#include <scene/Entity.hpp>


namespace ENGINE 
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);
		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
	
    
    private:
		void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectionContext;

	};

}