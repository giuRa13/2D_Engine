#include <Engine.hpp>
#include <entry.hpp>
#include "EditorLayer.hpp"

namespace ENGINE 
{

	class Editor : public Application
	{

	public:
		Editor()
			: Application("Editor")
		{
			PushLayer(new EditorLayer());
		}

		~Editor()
		{
		}
	};


	Application* CreateApplication()
	{
		return new Editor();
	}
}