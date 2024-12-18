#include "SandBox2D.hpp"
#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <renderer/OpenGL/OpenGLShader.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
    m_CheckerboardTexture = ENGINE::Texture2D::Create("../assets/textures/Checkerboard.png");
    m_CharTexture = ENGINE::Texture2D::Create("../assets/textures/ninja.png");
	m_MapTexture = ENGINE::Texture2D::Create("../assets/textures/16map.png");
	m_TileTexture = ENGINE::SubTexture2D::CreateFromCoords(m_MapTexture, { 13, 2}, {16, 16} );
}


void Sandbox2D::OnDetach()
{
}


void Sandbox2D::OnUpdate(ENGINE::TimeStep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

    if(ENGINE::Input::IsKeyPressed(ENGINE_KEY_J))
		m_SquarePosition.x -= m_SquareMoveSpeed *ts;
	else if(ENGINE::Input::IsKeyPressed(ENGINE_KEY_L))
		m_SquarePosition.x += m_SquareMoveSpeed *ts;
	if(ENGINE::Input::IsKeyPressed(ENGINE_KEY_K))
		m_SquarePosition.y -= m_SquareMoveSpeed *ts;
	else if (ENGINE::Input::IsKeyPressed(ENGINE_KEY_I))
		m_SquarePosition.y += m_SquareMoveSpeed *ts;

	static float rotation = 0.0f;
	rotation += ts * 50.0f;

	ENGINE::Renderer2D::ResetStats();

	// Render
	ENGINE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	ENGINE::RenderCommand::Clear();

	ENGINE::Renderer2D::BeginScene(m_CameraController.GetCamera());

    ENGINE::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f },  m_SquareColor);
    //ENGINE::Renderer2D::DrawQuad(m_SquarePosition, { 0.5f, 0.75f }, m_SquareColor);
	ENGINE::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture , 10.0f, {1.0, 0.8, 0.8, 1.0});
	ENGINE::Renderer2D::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, 5.0f);
	//ENGINE::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.9f, 0.5f, 0.2f, 1.0f });
	//ENGINE::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);

	ENGINE::Renderer2D::DrawQuad({ 0.0f, 0.5f, 0.0f }, { 1.0f, 1.0f }, m_MapTexture);
	ENGINE::Renderer2D::DrawQuad(m_SquarePosition, { 1.0f, 1.0f }, m_TileTexture);

	ENGINE::Renderer2D::EndScene();
}


void Sandbox2D::OnEvent(ENGINE::Event& e)
{
	m_CameraController.OnEvent(e);
}


void Sandbox2D::OnImGuiRender()
{

	ImGui::Begin("Settings");
		
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	auto stats = ENGINE::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	static bool showDemo = true;
	ImGui::ShowDemoWindow(&showDemo);

	ImGui::End();
}