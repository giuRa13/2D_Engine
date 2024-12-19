#include "EditorLayer.hpp"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <entt/entt.hpp>


namespace ENGINE
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{
	}

    void EditorLayer::OnAttach()
	{
		m_CheckerboardTexture = Texture2D::Create("../assets/textures/Checkerboard.png");
        //m_MapTexture = Texture2D::Create("../assets/textures/16map.png");
	    //m_TileTexture = SubTexture2D::CreateFromCoords(m_MapTexture, { 13, 2}, {16, 16} );

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		// Entities
		auto square = m_ActiveScene->CreateEntity("Green square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
		m_SquareEntity = square;

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;


		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				//auto& transform = GetComponent<TransformComponent>().Transform;
				//transform[3][0] = rand() % 10 - 5.0f;
			}
			void OnDestroy()
			{
			}
			void OnUpdate(TimeStep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				float speed = 5.0f;
				if (Input::IsKeyPressed(ENGINE_KEY_A))
					transform[3][0] -= speed * ts; // [3] = third column of the matrix, [0] = x
				if (Input::IsKeyPressed(ENGINE_KEY_D))
					transform[3][0] += speed * ts;
				if (Input::IsKeyPressed(ENGINE_KEY_W))
					transform[3][1] += speed * ts;
				if (Input::IsKeyPressed(ENGINE_KEY_S))
					transform[3][1] -= speed * ts;
			}
		};
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{

	}


	void EditorLayer::OnUpdate(TimeStep ts)
	{
		// Resize Viewport "Flash"
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
			{
				m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
				m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			}

		// Update
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		Renderer2D::ResetStats();

		// Render
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor(m_ClearColor);
		RenderCommand::Clear();
		//Renderer2D::BeginScene(m_CameraController.GetCamera());

		/*Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f },  m_SquareColor);
		//Renderer2D::DrawQuad(m_SquarePosition, { 0.5f, 0.75f }, m_SquareColor);
		Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture , 10.0f, {1.0, 0.8, 0.8, 1.0});
		Renderer2D::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, 5.0f);
		//Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.9f, 0.5f, 0.2f, 1.0f });
		//Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);
		Renderer2D::DrawQuad({ 0.0f, 0.5f, 0.0f }, { 1.0f, 1.0f }, m_MapTexture);
		Renderer2D::DrawQuad(m_SquarePosition, { 1.0f, 1.0f }, m_TileTexture);*/

		m_ActiveScene->OnUpdate(ts);

		//Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}


	void EditorLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();
		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();     
			}
			if (ImGui::BeginMenu("Demo"))
			{
				if (ImGui::MenuItem("Open")) showDemo = true;
				
				if (ImGui::MenuItem("Close")) showDemo = false;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if(showDemo)
			ImGui::ShowDemoWindow(&showDemo);

		////////////////////////////////////////////////
		m_SceneHierarchyPanel.OnImGuiRender();
		
		ImGui::Begin("Stats");

		auto stats = Renderer2D::GetStats();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(66, 150, 240, 240));
		ImGui::Text("Renderer2D Stats:");
		ImGui::PopStyleColor();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::NewLine();
		
		ImGui::Separator();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(66, 150, 240, 240));
		ImGui::Text("Clear Color");
		ImGui::PopStyleColor();
		ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
		if(ImGui::Button("Original"))
			m_ClearColor = {0.1f, 0.1f, 0.1f, 1};
		ImGui::NewLine();
		
		/*if (m_SquareEntity)
		{
			ImGui::Separator();
			auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(66, 150, 240, 240));
			ImGui::Text("%s", tag.c_str());
			ImGui::PopStyleColor();
			auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
			ImGui::NewLine();
			ImGui::Separator();
		}*/

		ImGui::Separator();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(66, 150, 240, 240));
		ImGui::Text("Camera Controlls");
		ImGui::PopStyleColor();
		ImGui::DragFloat3("Camera Transform",
			glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
		if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
		{
			m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
			m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
		}
		{
			auto& camera = m_SecondCamera.GetComponent<CameraComponent>().Camera;
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);
		}
		ImGui::NewLine();
		ImGui::Separator();

		ImGui::NewLine();
		uint64_t texID = m_CheckerboardTexture->GetRendererID();
		ImGui::Image(reinterpret_cast<void*>(texID), ImVec2{ 256.0f, 256.0f }, ImVec2{0, 1}, ImVec2{1, 0});
		ImGui::End();

		////////////////////////////////////////////////
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		/*if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}*/
		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{  m_ViewportSize.x,  m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

}