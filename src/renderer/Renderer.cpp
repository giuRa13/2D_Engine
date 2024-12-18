#include "Renderer.hpp"
#include "renderer/OpenGL/OpenGLShader.hpp"
#include "Renderer2D.hpp"


namespace ENGINE 
{
	//Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		//shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		//shader->UploadUniformMat4("u_Transform", transform);
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}
}