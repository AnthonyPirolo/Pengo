#pragma once
#include "Singleton.h"
#include "Command.h"
#include <SDL.h>
#define _X86_
#include <Xinput.h>
#include "Gamepad.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class Direction
		{
			Up, Down, Left, Right
		};

		struct KeyMappings
		{
			SDL_KeyCode m_KeyUp{};
			SDL_KeyCode m_KeyRight{};
			SDL_KeyCode m_KeyLeft{};
			SDL_KeyCode m_KeyDown{};
		};

		std::vector<std::pair<std::unique_ptr<MoveCommand>, KeyMappings>> m_MovementControllers;

		bool ProcessInput();

		void MakeMoveCommandWithOwner(GameObject* object, float speed = 50.0f);
		void BindMoveCommand(SDL_KeyCode key, Direction direction, GameObject* object);
	private:
		//Gamepad m_GamePad;
	};

}