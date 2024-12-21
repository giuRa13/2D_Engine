#pragma once

#include "Engine.hpp"
#include "panels/SceneHierarchyPanel.hpp"


namespace ENGINE 
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;

		void OnEvent(Event& e) override;


	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();

		ENGINE::OrthographicCameraController m_CameraController;
		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1 };
		bool showDemo = false;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = true;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		int m_GizmoType = -1;
		
		Ref<Texture2D> m_CheckerboardTexture;
        Ref<Texture2D> m_MapTexture;
	    Ref<SubTexture2D> m_TileTexture;
		
        //glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	    //glm::vec3 m_SquarePosition = { 0.5f, -0.5f, 0.2 };
	    //float m_SquareMoveSpeed = 1.0f;

	};

}