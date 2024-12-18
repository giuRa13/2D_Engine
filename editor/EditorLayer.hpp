#pragma once

#include "Engine.hpp"


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
		ENGINE::OrthographicCameraController m_CameraController;
		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1 };

		Ref<Scene> m_ActiveScene;
		entt::entity m_SquareEntity;
		
		Ref<Texture2D> m_CheckerboardTexture;
        ENGINE::Ref<ENGINE::Texture2D> m_MapTexture;
	    ENGINE::Ref<ENGINE::SubTexture2D> m_TileTexture;
		
        glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	    glm::vec3 m_SquarePosition = { 0.5f, -0.5f, 0.2 };
	    float m_SquareMoveSpeed = 1.0f;
		bool showDemo = false;

	};

}