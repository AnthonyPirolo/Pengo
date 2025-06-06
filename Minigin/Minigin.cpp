#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Time.h"
#include <chrono>
#include <thread> 
#include "SDLSoundSystem.h"
#include "ServiceLocator.h"
#include "GameTime.h"
#include "XInputManager.h"
#include "CollisionSystem.h"

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

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    g_window = SDL_CreateWindow(
        "Programming 4 assignment",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        446,
        576,
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



    Renderer::GetInstance().Init(g_window);
    ResourceManager::GetInstance().Init(dataPath);
    ServiceLocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());
	ServiceLocator::RegisterCollisionSystem(std::make_unique<CollisionSystem>());
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
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
    auto& time = GameTime::GetInstance();
    auto& xi = dae::XInputManager::GetInstance();

    constexpr float fixed_time_step = 0.016f; // 60 FPS
    constexpr int   ms_per_frame = 16;     // ~16 ms

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

        bool xiOk = xi.ProcessInput();

        bool sdlOk = input.ProcessInput();

        doContinue = (sdlOk && xiOk);

        while (lag >= fixed_time_step)
        {
            sceneManager.FixedUpdate(fixed_time_step);
            lag -= fixed_time_step;
        }

        sceneManager.Update();
        sceneManager.LateUpdate();

        renderer.Render();

        const auto frame_end_time = current_time + std::chrono::milliseconds(ms_per_frame);
        std::this_thread::sleep_until(frame_end_time);
    }
}