#include "Framebuffer.hpp"
#include "Renderer.hpp"
#include "OpenGL/OpenGLFramebuffer.hpp"
#include "Log.hpp"

namespace ENGINE 
{
	
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    ENGINE_CORE_WARN("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}
		return nullptr;
	}
}