#include "application.hpp"
#include "Log.hpp"
#include "Input.hpp"
#include "TimeStep.hpp"
#include "events/Event.hpp"
#include "events/KeyEvent.hpp"
#include "events/KeyCode.hpp"
#include "events/MouseCodes.hpp"

#include "layers/Layer.hpp"
#include "layers/ImGuiLayer.hpp"

#include "renderer/Renderer.hpp"
#include "renderer/Renderer2D.hpp"
#include "renderer/RenderCommand.hpp"
#include "renderer/Buffer.hpp"
#include "renderer/Shader.hpp"
#include "renderer/SubTexture2D.hpp"
#include "renderer/VertexArray.hpp"
#include "renderer/OrthographicCamera.hpp"
#include "renderer/OrthoCameraController.hpp"
#include "renderer/Framebuffer.hpp"


//#define ENGINE_BIND_EVENT_FUNC(fn) std::bind(&fn, this, std::placeholders::_1)
//#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)
//#define BIND_EVENT_FUNC(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }