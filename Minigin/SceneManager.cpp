#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate(float deltaTime)
{
	for(auto& scene : m_scenes)
	{
		scene->FixedUpdate(deltaTime);
	}
}

void dae::SceneManager::Update()
{
	for (const auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::LateUpdate()
{
	for (const auto& scene : m_scenes)
	{
		scene->LateUpdate();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
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
