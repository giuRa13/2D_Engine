#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window;

namespace ENGINE
{


class ImguiEx
{
  public:
	bool Initialize();
	void Begin();
	void End();
	void Render();

	void Shutdown();
	bool ShowDemo = false;

  private:
	void MainMenu();
	void SetStyle();
};
} // namespace ENGINE
