#include <SDL.h>
#include "InputManager.h"
#include "imgui_impl_sdl2.h"

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}

		// Process IMGUI events
		// ImGui_ImplSDL2_ProcessEvent(&e);
	}

	// Process movement every frame, regardless of whether an event happened
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);
	for (auto& controller : m_MovementControllers)
	{
		glm::vec3 moveDirection{ 0.f, 0.f, 0.f };

		if (keyState[SDL_GetScancodeFromKey(controller.second.m_KeyUp)])
			moveDirection.y -= 1.f;
		if (keyState[SDL_GetScancodeFromKey(controller.second.m_KeyRight)])
			moveDirection.x += 1.f;
		if (keyState[SDL_GetScancodeFromKey(controller.second.m_KeyDown)])
			moveDirection.y += 1.f;
		if (keyState[SDL_GetScancodeFromKey(controller.second.m_KeyLeft)])
			moveDirection.x -= 1.f;

		if (moveDirection != glm::vec3{ 0.f, 0.f, 0.f }) {
			controller.first->SetDirection(moveDirection);
			controller.first->Execute();
		}
	}

	return true;
}

void dae::InputManager::MakeMoveCommandWithOwner(GameObject* object, float speed)
{
	for (auto& controller : m_MovementControllers)
	{
		if (object == controller.first.get()->GetGameObject()) return;
	}

	KeyMappings newKeyMap = {};
	std::unique_ptr<MoveCommand> newMoveComm = std::make_unique<MoveCommand>(object, speed);
    auto newController = std::make_pair(std::move(newMoveComm), newKeyMap);
	m_MovementControllers.push_back(std::move(newController));
}

void dae::InputManager::BindMoveCommand(SDL_KeyCode key, Direction direction, GameObject* object)
{
	for (auto& controller : m_MovementControllers)
	{
		if (object == controller.first.get()->GetGameObject())
		{
			switch (direction)
			{
			case Direction::Up:
				controller.second.m_KeyUp = key;
				break;
			case Direction::Right:
				controller.second.m_KeyRight = key;
				break;
			case Direction::Down:
				controller.second.m_KeyDown = key;
				break;
			case Direction::Left:
				controller.second.m_KeyLeft = key;
				break;
			}
		}
	}
}