#pragma once

#include <memory>
#include "events/Event.hpp"
#include "events/ApplicationEvent.hpp"
#include "Window.hpp"
#include "TimeStep.hpp"
#include "layers/LayerStack.hpp"
#include "layers/ImGuiLayer.hpp"
//#include "renderer/Shader.hpp"
//#include "renderer/Buffer.hpp"
//#include "renderer/VertexArray.hpp"
//#include "renderer/OrthographicCamera.hpp"



namespace ENGINE
{
	class Application
	{
	public:
		Application(const std::string& name = "Engine App");
		virtual ~Application() = default;
		
        void Run();
        void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		void Close();


    private:
        std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		
		static Application* s_Instance;
        bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	};

	// To be defined in CLIENT
	Application* CreateApplication();
}