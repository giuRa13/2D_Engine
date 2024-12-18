#pragma once

#include <memory>
#include "application.hpp"
#include "Log.hpp"

#ifdef _WIN32
	#include <Windows.h>
#endif

extern ENGINE::Application* ENGINE::CreateApplication();

int main(int argc, char** argv)
{
    ENGINE::Log::Init();
    ENGINE_CORE_INFO("Logger Initialized");

	auto app = ENGINE::CreateApplication();
	app->Run();
	delete app;
}