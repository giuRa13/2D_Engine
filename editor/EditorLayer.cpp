#include "EditorLayer.hpp"
#include "FileDialog.hpp"
#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <entt/entt.hpp>
#include <scene/SceneSerializer.hpp>
#include <scene/Math.hpp>



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
#if 0
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
				//auto& translation = GetComponent<TransformComponent>().Translation;
				//translation.x = rand() % 10 - 5.0f;
			}
			void OnDestroy()
			{
			}
			void OnUpdate(TimeStep ts)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(ENGINE_KEY_A))
					translation.x -= speed * ts; // [3] = third column of the matrix, [0] = x
				if (Input::IsKeyPressed(ENGINE_KEY_D))
					translation.x  += speed * ts;
				if (Input::IsKeyPressed(ENGINE_KEY_W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(ENGINE_KEY_S))
					translation.y -= speed * ts;
			}
		};
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
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

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(ENGINE_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if(e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch(e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();
				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();
				break;
			}
			case Key::S:
			{
				if (control)
					SaveSceneAs();
				break;
			}
			// Gizmos
			case Key::Q:
				m_GizmoType = -1;
				break;
			case Key::W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialog::OpenFile({".engine"} );

		if(!filepath.empty())
		{
			ENGINE_CORE_INFO("Serializing Scene...");

			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}
	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialog::SaveFile({".engine"} );

		if(!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}


	// ImGui ///////////////////////////////////////////////////////////////////////////7
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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f; // if is a docking window
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSize; // set back to default
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) 
				{
					NewScene();
				}
				if (ImGui::MenuItem("Open", "Ctrl+O")) 
				{
					OpenScene();
				}
				if (ImGui::MenuItem("Save As...", "Ctrl+S")) 
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit")) Application::Get().Close();

				ImGui::EndMenu();     
			}

			if (ImGui::BeginMenu("Demo"))
			{
				if (ImGui::MenuItem("Open")) showDemo = true;
				
				if (ImGui::MenuItem("Close")) showDemo = false;

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Theme"))
			{
				auto imGuiLayer = Application::Get().GetImGuiLayer();

				if (ImGui::MenuItem("Theme 1"))
				{
					imGuiLayer->SetDefaultTheme();
					imGuiLayer->SetThemeColors();
				} 
				if (ImGui::MenuItem("Theme 2")) imGuiLayer->SetTheme2();
				if (ImGui::MenuItem("Default")) imGuiLayer->SetDefaultTheme();

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

		ImGui::NewLine();
		ImGui::SameLine(ImGui::GetWindowWidth() /6);
		uint64_t texID = m_CheckerboardTexture->GetRendererID();
		ImGui::Image(reinterpret_cast<void*>(texID), ImVec2{ 256.0f, 256.0f }, ImVec2{0, 1}, ImVec2{1, 0});
		ImGui::End();

		////////////////////////////////////////////////
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{  m_ViewportSize.x,  m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

		// ImGuizmo
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();
			/*ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(transform));
			if(ImGuizmo::IsUsing())
			{
				tc.Translation = glm::vec3(transform[3]);
			}*/

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;
			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(
				glm::value_ptr(cameraView), 
				glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, 
				ImGuizmo::LOCAL, 
				glm::value_ptr(transform),
				nullptr, 
				snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

}