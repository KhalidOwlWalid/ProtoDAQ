#pragma once

#ifndef IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#define IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#endif

#include "implot.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif
#ifdef _WIN32
#include <windows.h>        // SetProcessDPIAware()
#endif

#include "protodaq_application.hpp"

namespace ProtoDAQ {

class ImDAQ: public App {

public:
    ImDAQ();
    ~ImDAQ();

    bool init(const std::vector<DAQ_Protocol> protocols) override;
    void run() override;
    void shutdown() override;

private:
    SDL_Window *window;
    SDL_WindowFlags window_flags;
    SDL_GLContext gl_context;
    // ImGuiIO &io = ImGui::GetIO();
    // ImGuiStyle &style = ImGui::GetStyle();
    ImGuiIO io;
    ImGuiStyle style;
    ImFont *font;
    ImVec4 clear_color;

};

}