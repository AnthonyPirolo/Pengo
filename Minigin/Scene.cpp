#include "Scene.h"
#include <iostream>
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name)
    : m_name(name)
{
}

Scene::~Scene()
{
    m_objects.clear();
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
    m_objects.push_back(std::move(object));
}

void Scene::Remove(const std::shared_ptr<GameObject>& object)
{
    m_objects.erase(
        std::remove(m_objects.begin(), m_objects.end(), object),
        m_objects.end()
    );
}

void Scene::FixedUpdate(float deltaTime)
{
    for (auto& object : m_objects)
    {
        object->FixedUpdate(deltaTime);
    }
}

void Scene::Update()
{
    for (auto& object : m_objects)
    {
        object->Update();
    }
}

void Scene::RemoveAll()
{
    m_PendingClear = true;
}

void Scene::LateUpdate()
{
    for (auto& object : m_objects)
    {
        object->LateUpdate();
    }

    m_objects.erase(
        std::remove_if(
            m_objects.begin(),
            m_objects.end(),
            [](const std::shared_ptr<GameObject>& obj)
            {
                return obj->IsMarkedForDestroy();
            }
        ),
        m_objects.end()
    );

    if (m_PendingClear)
    {
        m_objects.clear();
        m_PendingClear = false;
    }
}

void Scene::Render() const
{
    for (const auto& object : m_objects)
    {
        object->Render();
    }
}

std::vector<std::shared_ptr<GameObject>> Scene::GetGameObjects() const
{
    return m_objects;
}

std::vector<std::shared_ptr<GameObject>> Scene::GetObjectsWithTag(const std::string& tag) const
{
    std::vector<std::shared_ptr<GameObject>> matches;
    matches.reserve(m_objects.size());

    for (const auto& obj : m_objects)
    {
        if (!obj) continue;
        if (obj->GetTag() == tag)
        {
            matches.push_back(obj);
        }
    }
    return matches;
}