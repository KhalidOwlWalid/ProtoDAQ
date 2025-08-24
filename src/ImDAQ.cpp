#include "../include/ImDAQ.hpp"

using namespace ProtoDAQ;

ImDAQ::ImDAQ() {
    spdlog::info("ImDAQ application");
    // io = ImGui::GetIO(); (void)io;
    // style = ImGui::GetStyle();
}

ImDAQ::~ImDAQ() {
    spdlog::info("Closing ImDAQ");
}

bool ImDAQ::init(const std::vector<DAQ_Protocol> protocols) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
    window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)(1280 * main_scale), (int)(720 * main_scale), window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr)
    {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return -1;
    }

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    spdlog::info("Currently here");
    io = std::move(ImGui::GetIO());
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup scaling
    style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    style.FontSizeBase = 10.0f;
    font = io.Fonts->AddFontFromFileTTF("/home/khalidowlwalid/Documents/KhalidOWlWalid-Github-Projects/OwlCppWorkspace/imgui_test/misc/fonts/DroidSans.ttf");

    // Our state
    bool show_demo_window = true;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Initializing the backend for protocols we require
    for (size_t i = 0; i < protocols.size(); i++) {
        std::unique_ptr<DAQ_Backend> backend_ptr;
        backend_ptr = _create_backend(protocols[i]);

        if (backend_ptr == nullptr) {
            spdlog::warn("Fails to create backend for this protocol");
            continue;
        } else {
            backend_ptr->init();
        }

        _daq_backend_manager.push_back(std::move(backend_ptr));
    }

    _running = true;

    return true;
}

void ImDAQ::run() {

    while (_running) {

        // ImGui::SetCurrentContext();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                _running = false;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                _running = false;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        ImPlot::CreateContext();
        // ImPlot::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

        for (size_t i = 0; i < _daq_backend_manager.size(); i++) {
            _daq_backend_manager[i]->update();
        }

    }

}

void ImDAQ::shutdown() {
    for (size_t i = 0; i < _daq_backend_manager.size(); i++) {
        _daq_backend_manager[i]->shutdown();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// void ImDAQ::Demo_RealtimePlots() {
//     ImGui::BulletText("Move your mouse to change the data!");
//     ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
//     static ScrollingBuffer sdata1, sdata2;
//     static RollingBuffer   rdata1, rdata2;
//     ImVec2 mouse = ImGui::GetMousePos();
//     static float t = 0;
//     t += ImGui::GetIO().DeltaTime;
//     sdata1.AddPoint(t, mouse.x * 0.0005f);
//     rdata1.AddPoint(t, mouse.x * 0.0005f);
//     sdata2.AddPoint(t, mouse.y * 0.0005f);
//     rdata2.AddPoint(t, mouse.y * 0.0005f);

//     static float history = 10.0f;
//     ImGui::SliderFloat("History",&history,1,30,"%.1f s");
//     rdata1.Span = history;
//     rdata2.Span = history;

//     static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

//     if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1,150))) {
//         ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
//         ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
//         ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
//         ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
//         ImPlot::PlotShaded("Mouse X", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), -INFINITY, 0, sdata1.Offset, 2 * sizeof(float));
//         ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2*sizeof(float));
//         ImPlot::EndPlot();
//     }
//     if (ImPlot::BeginPlot("##Rolling", ImVec2(-1,150))) {
//         ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
//         ImPlot::SetupAxisLimits(ImAxis_X1,0,history, ImGuiCond_Always);
//         ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
//         ImPlot::PlotLine("Mouse X", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 0, 2 * sizeof(float));
//         ImPlot::PlotLine("Mouse Y", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 0, 2 * sizeof(float));
//         ImPlot::EndPlot();
//     }
// }
