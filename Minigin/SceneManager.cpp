#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate(float deltaTime)
{
    if (!m_scenes.empty())
    {
        m_scenes.back()->FixedUpdate(deltaTime);
    }
}

void dae::SceneManager::Update()
{
    if (!m_scenes.empty())
    {
        m_scenes.back()->Update();
    }
}

void dae::SceneManager::LateUpdate()
{
    if (!m_scenes.empty())
    {
        m_scenes.back()->LateUpdate();
    }
}

void dae::SceneManager::Render()
{
    if (!m_scenes.empty())
    {
        m_scenes.back()->Render();
    }
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
    const auto& scene = std::shared_ptr<Scene>(new Scene(name));
    m_scenes.push_back(scene);
    return *scene;
}

dae::Scene& dae::SceneManager::GetActiveScene()
{
    return *m_scenes.back();
}