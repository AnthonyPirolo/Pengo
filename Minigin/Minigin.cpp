#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "imgui_impl_opengl3_loader.h"
#include <SDL_opengl.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Time.h"
#include <chrono>
#include <thread> 
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "GameObject.h"
#include <GL/gl.h>

SDL_Window* g_window{};
SDL_GLContext g_glContext{};

void PrintSDLVersion()
{
    SDL_version version{};
    SDL_VERSION(&version);
    printf("We compiled against SDL version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    SDL_GetVersion(&version);
    printf("We are linking against SDL version %u.%u.%u.\n",
        version.major, version.minor, version.patch);

    SDL_IMAGE_VERSION(&version);
    printf("We compiled against SDL_image version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    version = *IMG_Linked_Version();
    printf("We are linking against SDL_image version %u.%u.%u.\n",
        version.major, version.minor, version.patch);

    SDL_TTF_VERSION(&version);
    printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    version = *TTF_Linked_Version();
    printf("We are linking against SDL_ttf version %u.%u.%u.\n",
        version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string& dataPath)
{
    PrintSDLVersion();
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    g_window = SDL_CreateWindow(
        "Programming 4 assignment",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_OPENGL
    );

    if (g_window == nullptr)
    {
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
    }

    g_glContext = SDL_GL_CreateContext(g_window);
    if (g_glContext == nullptr)
    {
        throw std::runtime_error(std::string("SDL_GL_CreateContext Error: ") + SDL_GetError());
    }

    SDL_GL_MakeCurrent(g_window, g_glContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(g_window, g_glContext);
    ImGui_ImplOpenGL3_Init("#version 130");

    Renderer::GetInstance().Init(g_window);
    ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(g_glContext);
    SDL_DestroyWindow(g_window);
    g_window = nullptr;
    SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
    load();

    auto& renderer = Renderer::GetInstance();
    auto& sceneManager = SceneManager::GetInstance();
    auto& input = InputManager::GetInstance();
	auto& time = Time::GetInstance();

    constexpr float fixed_time_step = 0.016f; // Assuming 60 FPS
    constexpr int ms_per_frame = 16;          // Assuming 16 ms per frame

    bool doContinue = true;
    auto last_time = std::chrono::high_resolution_clock::now();
    float lag = 0.0f;

    while (doContinue)
    {
        const auto current_time = std::chrono::high_resolution_clock::now();
        const float delta_time = std::chrono::duration<float>(current_time - last_time).count();
		time.SetDeltaTime(delta_time);
        last_time = current_time;
        lag += delta_time;

        // --- EVENT HANDLING ---
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT ||
                (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(g_window)))
            {
                doContinue = false;
            }
        }

        if (!doContinue) break; // Ensure immediate exit before processing input

        doContinue = input.ProcessInput();

        // --- UPDATE LOGIC ---
        while (lag >= fixed_time_step)
        {
            sceneManager.FixedUpdate(fixed_time_step);
            lag -= fixed_time_step;
        }
		sceneManager.Update();
		sceneManager.LateUpdate();

		//Render Game Objects
        renderer.Render();

        // --- RENDERING ---
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Clear the screen
        //glClear(GL_COLOR_BUFFER_BIT);

        //// 3️ Render ImGui overlay
        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplSDL2_NewFrame();
        //ImGui::NewFrame();

        //ImGui::Begin("GameObject Manager", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        //static char name[128] = "";
        //ImGui::InputText("Name", name, IM_ARRAYSIZE(name));

        //if (ImGui::Button("Add GameObject"))
        //{
        //    auto gameObject = std::make_shared<dae::GameObject>();
        //    sceneManager.GetActiveScene().Add(gameObject);
        //}

        //if (ImGui::Button("Remove All GameObjects"))
        //{
        //    sceneManager.GetActiveScene().RemoveAll();
        //}

        //ImGui::Separator();

        //for (auto& gameObject : sceneManager.GetActiveScene().GetGameObjects())
        //{
        //    ImGui::Text("GameObject: %s", name);
        //    if (ImGui::Button("Add Texture Component"))
        //    {
        //        gameObject->AddTextureComponent("texture.png");
        //    }
        //    if (ImGui::Button("Add Text Component"))
        //    {
        //        auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
        //        gameObject->AddTextComponent("Sample Text", font);
        //    }
        //    if (ImGui::Button("Remove All Components"))
        //    {
        //        gameObject->RemoveAllComponents();
        //    }
        //    ImGui::Separator();
        //}

        //ImGui::End();

        //// 4️ Render ImGui last (on top)
        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //Swap buffers (FINAL STEP)
        //SDL_GL_SwapWindow(g_window);

        // Ensure 60 FPS cap
        const auto frame_end_time = current_time + std::chrono::milliseconds(ms_per_frame);
        std::this_thread::sleep_until(frame_end_time);
    }
}