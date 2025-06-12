#pragma once
#include <memory>
#include <iostream>
#include "ISoundSystem.h"
#include "ICollisionSystem.h"
#include "InputManager.h"
#include "XInputManager.h"
#include "NullSystems.h"

class ServiceLocator final
{
public:
    static void Initialize()
    {
        m_SSInstance = nullptr;
        m_CollisionInstance = nullptr;
        m_InputInstance = nullptr;
        m_XInputInstance = nullptr;
    }

    static ISoundSystem& GetSoundSystem()
    {
        return m_SSInstance ? *m_SSInstance : s_NullSoundSystem;
    }

    static dae::ICollisionSystem& GetCollisionSystem()
    {
        return m_CollisionInstance ? *m_CollisionInstance : s_NullCollisionSystem;
    }

    static dae::InputManager& GetInputManager()
    {
        return m_InputInstance ? *m_InputInstance : s_NullInputManager;
    }

    static dae::XInputManager& GetXInputManager()
    {
        return m_XInputInstance ? *m_XInputInstance : s_NullXInputManager;
    }

    static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& ss)
    {
        m_SSInstance = std::move(ss);
    }

    static void RegisterCollisionSystem(std::unique_ptr<dae::ICollisionSystem>&& cs)
    {
        m_CollisionInstance = std::move(cs);
    }

    static void RegisterInputManager(std::unique_ptr<dae::InputManager>&& im)
    {
        m_InputInstance = std::move(im);
    }

    static void RegisterXInputManager(std::unique_ptr<dae::XInputManager>&& xim)
    {
        m_XInputInstance = std::move(xim);
    }

    static dae::IFileService& GetFileService() {
        return m_FileServiceInstance ? *m_FileServiceInstance : s_NullFileService;
    }

    static void RegisterFileService(std::unique_ptr<dae::IFileService>&& fs) {
        m_FileServiceInstance = std::move(fs);
    }

private:
    static std::unique_ptr<ISoundSystem> m_SSInstance;
    static std::unique_ptr<dae::ICollisionSystem> m_CollisionInstance;
    static std::unique_ptr<dae::InputManager> m_InputInstance;
    static std::unique_ptr<dae::XInputManager> m_XInputInstance;
    static std::unique_ptr<dae::IFileService> m_FileServiceInstance;

    static dae::NullSoundSystem s_NullSoundSystem;
    static dae::NullCollisionSystem s_NullCollisionSystem;
    static dae::NullInputManager s_NullInputManager;
    static dae::NullXInputManager s_NullXInputManager;
    static dae::NullFileService s_NullFileService;
};