#include "ImGuiEx.hpp"
#include "application.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Window.hpp"
#include <imgui_internal.h>
#include <iostream>
#include "Window.hpp"

namespace ENGINE
{
bool ImguiEx::Initialize()
{
	// Initialize Imgui
	const char* glsl_version = "#version 450";
	IMGUI_CHECKVERSION();

	if (!ImGui::CreateContext())
	{
		std::cout <<"Failed to create imgui context!\n";
		return false;
	}

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Only want the windows to be moved by the titlebar
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	// Setup Platform/Renderer backends
    Application& app = Application::Get();
	GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
	{
		std::cout <<"Failed to initialize ImGuiGLFW_InitForOpenGL!\n";
		return false;
	}

	if (!ImGui_ImplOpenGL3_Init(glsl_version))
	{
		std::cout <<"Failed to initialize ImGui_OpenGL3!\n";
		return false;
	}

	//SetStyle();

	return true;
}

void ImguiEx::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImguiEx::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImguiEx::Render()
{
	MainMenu();
}


void ImguiEx::MainMenu()
{
	const auto dockSpaceId = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	if (static auto bFirstTime = true; bFirstTime) [[unlikely]]
	{
		bFirstTime = false;
		ImGui::DockBuilderRemoveNode(dockSpaceId);
		ImGui::DockBuilderAddNode(dockSpaceId);

		/*auto centerNodeId = dockSpaceId;
		const auto leftNodeId = ImGui::DockBuilderSplitNode(centerNodeId, ImGuiDir_Left, 0.20f, nullptr, &centerNodeId);
		const auto bottomNodeId =
			ImGui::DockBuilderSplitNode(centerNodeId, ImGuiDir_Down, 0.25f, nullptr, &centerNodeId);*/

		auto centerNodeId = dockSpaceId;
        const auto leftNodeId = ImGui::DockBuilderSplitNode(centerNodeId, ImGuiDir_Left, 0.2f, nullptr, &centerNodeId);
        const auto rightNodeId = ImGui::DockBuilderSplitNode(centerNodeId, ImGuiDir_Right, 0.2f, nullptr, &centerNodeId);
        const auto LogNodeId = ImGui::DockBuilderSplitNode(centerNodeId, ImGuiDir_Down, 0.28f, nullptr, &centerNodeId);

		ImGui::DockBuilderDockWindow("Side Display", leftNodeId);
		ImGui::DockBuilderDockWindow("Log Display", LogNodeId);
		ImGui::DockBuilderDockWindow("Content Display", centerNodeId);
		/*ImGui::DockBuilderDockWindow("Side Display", leftNodeId);
		ImGui::DockBuilderDockWindow("Log Display", bottomNodeId);
		ImGui::DockBuilderDockWindow("Content Display", centerNodeId);*/

		ImGui::DockBuilderFinish(dockSpaceId);
	}

	/*if (ImGui::BeginMainMenuBar())
	{
		ImGui::EndMainMenuBar();
	}*/

	if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Create")) { }
            if (ImGui::MenuItem("Open", "Ctrl+O")) { }
        	if (ImGui::MenuItem("Save", "Ctrl+S")) { }
            if (ImGui::MenuItem("Save as..")) { }
             ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }

}

void ImguiEx::Shutdown()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}
}