#include "FileDialog.hpp"
#include <application.hpp>
#include <iostream>

#include "nfd.hpp"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
//#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>
#include <nfd_glfw3.h>


namespace ENGINE
{

    std::string FileDialog::OpenFile(const char* filter)
    {
        //NFD::Guard nfdGuard;

        nfdu8char_t *outPath;
        nfdu8filteritem_t filters[2] = { { "Source code", "c,cpp,cc,engine" }, { "Headers", "h,hpp" } };
        nfdopendialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 2;

        GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        NFD_GetNativeWindowFromGLFWWindow(window, &args.parentWindow);

        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
        if (result == NFD_OKAY)
        {
            return std::string(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            //puts("User pressed cancel.");
        }
        else 
        {
            printf("Error: %s\n", NFD_GetError());
        }

        return std::string();
    }


    std::string FileDialog::SaveFile(const char* filter)
    {
        nfdu8char_t *outPath;
        nfdu8filteritem_t filters[2] = { { "Source code", "c,cpp,cc,engine" }, { "Headers", "h,hpp" } };
        nfdsavedialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 2;
        args.defaultName = "Untitled.engine";

        GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        NFD_GetNativeWindowFromGLFWWindow(window, &args.parentWindow);

        nfdresult_t result = NFD_SaveDialogU8_With(&outPath,  &args);
        if (result == NFD_OKAY)
        {
            return std::string(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            //puts("User pressed cancel.");
        }
        else 
        {
            printf("Error: %s\n", NFD_GetError());
        }

        return std::string();
    }

}

