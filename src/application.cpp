#include "application.hpp"
#include "events/ApplicationEvent.hpp"
#include "Log.hpp"
#include "Input.hpp"
#include "WindowInput.hpp"
#include "events/KeyCode.hpp"
#include "renderer/Renderer.hpp"
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>



namespace ENGINE
{
    #define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)
    //#define BIND_EVENT_FUNC(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

    Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name) 
	{
        s_Instance = this;

        //m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window = Window::Create(WindowProps(name));
        m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));
        //m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		
		//m_Window->SetVSync(false);

		Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
	}


    void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}


    void Application::OnEvent(Event& e)
    {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OnWindowResize));
		//ENGINE_CORE_TRACE("{}", e.ToString());

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
    }


	void Application::Run()
	{   
		while (m_Running)
        {
            if(Input::IsKeyPressed(Key::Escape))
                m_Running = false;

			float time = (float)glfwGetTime();
			TimeStep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timeStep);
			}

            
            m_ImGuiLayer->Begin();
            {    
                for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();


			m_Window->OnUpdate();
        }
	}

	
	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}




/*ENGINE::Application* ENGINE::CreateApplication()
{
	return new Application();
}*/