//﻿#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components.h"
#include "InputManager.h"
#include <SDL.h>
#include "LivesDisplay.h"
#include "PointsDisplay.h"
#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "SoundPlayer.h"

void BindMovementCommands(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode up, SDL_KeyCode right, SDL_KeyCode left, SDL_KeyCode down);
void BindAttackCommmand(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode attackKey);
void BindAddPointsCommand(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode addPointsKey);
void MakeLivesDisplay(std::shared_ptr<dae::GameObject> actor, std::shared_ptr<dae::GameObject> livesDisplayObj, dae::HealthComponent* healthComponent, dae::Scene* scene);
void MakePointsDisplay(std::shared_ptr<dae::GameObject> actor, std::shared_ptr<dae::GameObject> pointsDisplayObj, dae::PointsComponent* pointsComponent, dae::Scene* scene);
void BindSoundTestCommand(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode soundKey, int ID);

void load()
{	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto soundPlayer = std::make_shared < dae::SoundPlayer>();
	/*auto& soundSystem = ServiceLocator::GetSoundSystem();
	std::cout << "Registered Sound System at: " << &ServiceLocator::GetSoundSystem() << std::endl;
	soundSystem.LoadSound("TestSound.wav", 1);
	soundSystem.LoadSound("TestSound2.wav", 2);*/

	auto background = std::make_shared<dae::GameObject>();
	background->AddComponent<dae::TextureComponent>(background.get(), "LevelBG.png");
	background->GetComponent<dae::TextureComponent>()->SetScale(2.f, 2.f);
	background->SetLocalPosition(glm::vec3(0, 47, 0));
	scene.Add(background);

	auto fps = std::make_shared<dae::GameObject>();
	fps->AddComponent<dae::TextComponent>(fps.get(), "FPS: 0", font);
	fps->AddComponent<dae::FpsComponent>(fps.get());
	fps->SetLocalPosition(glm::vec3(10, 20, 0));
	scene.Add(fps);

	auto P1 = std::make_shared<dae::GameObject>();
	P1->AddComponent<dae::TextureComponent>(P1.get(), "SinglePenguin.png");
	P1->GetComponent<dae::TextureComponent>()->SetScale(2.f, 2.f);
	P1->SetLocalPosition(glm::vec3(500, 300, 0));
	scene.Add(P1);
	auto P1Health = P1->AddComponent<dae::HealthComponent>(P1.get());
	P1Health->SetHealth(3);
	auto P1Points = P1->AddComponent<dae::PointsComponent>(P1.get());
	P1Points->AttachObserver(soundPlayer);
	//rot->AddComponent<dae::RotationComponent>(rot.get(), 360.f , glm::vec3(300, 300, 0));

	BindMovementCommands(P1, SDLK_w, SDLK_d, SDLK_a, SDLK_s);
	BindAttackCommmand(P1, SDLK_SPACE);
	BindAddPointsCommand(P1, SDLK_p);
	BindSoundTestCommand(P1, SDLK_k, 1); //test
	BindSoundTestCommand(P1, SDLK_l, 2); //test


	auto rot2 = std::make_shared<dae::GameObject>();
	rot2->SetLocalPosition(glm::vec3(50, 300, 0));
	rot2->AddComponent<dae::TextureComponent>(rot2.get(), "SinglePenguin2.png");
	scene.Add(rot2);
	auto rot2Health = rot2->AddComponent<dae::HealthComponent>(rot2.get());
	rot2Health->SetHealth(3);
	auto rot2Points = rot2->AddComponent<dae::PointsComponent>(rot2.get());
	rot2Points->AttachObserver(soundPlayer);
	//rot2->AddComponent<dae::RotationComponent>(rot2.get(), -180.f, rot.get());

	BindMovementCommands(rot2, SDLK_UP, SDLK_RIGHT, SDLK_LEFT, SDLK_DOWN);
	BindAttackCommmand(rot2, SDLK_RETURN);
	BindAddPointsCommand(rot2, SDLK_o);

	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto livesDisplayObj1 = std::make_shared<dae::GameObject>();
	livesDisplayObj1->SetLocalPosition(glm::vec3(50, 200, 0));
	MakeLivesDisplay(P1, livesDisplayObj1, P1Health, &scene);

	auto livesDisplayObj2 = std::make_shared<dae::GameObject>();
	livesDisplayObj2->SetLocalPosition(glm::vec3(500, 200, 0));
	MakeLivesDisplay(rot2, livesDisplayObj2, rot2Health, &scene);

	auto instructions = std::make_shared<dae::GameObject>();
	instructions->SetLocalPosition(glm::vec3(10, 430, 0));
	instructions->AddComponent<dae::TextComponent>(instructions.get(), "Player 1: WASD to move, Space to attack. Press p to add points", font2);
	scene.Add(instructions);

	auto instructions2 = std::make_shared<dae::GameObject>();
	instructions2->SetLocalPosition(glm::vec3(10, 450, 0));
	instructions2->AddComponent<dae::TextComponent>(instructions2.get(), "Player 2: Arrow keys to move, Enter to attack. Press o to add points", font2);
	scene.Add(instructions2);

	auto pointsDisplayObj1 = std::make_shared<dae::GameObject>();
	pointsDisplayObj1->SetLocalPosition(glm::vec3(50, 230, 0));
	MakePointsDisplay(P1, pointsDisplayObj1, P1Points, &scene);

	auto pointsDisplayObj2 = std::make_shared<dae::GameObject>();
	pointsDisplayObj2->SetLocalPosition(glm::vec3(500, 230, 0));
	MakePointsDisplay(rot2, pointsDisplayObj2, rot2Points, &scene);

	// W03 ImGui Plot results
	/*auto imguiPlot = std::make_shared<dae::GameObject>();
	imguiPlot->AddComponent<dae::ImGuiPlotComponent>(imguiPlot.get());
	scene.Add(imguiPlot);*/

}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}

void BindMovementCommands(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode up, SDL_KeyCode right, SDL_KeyCode left, SDL_KeyCode down) //Change to desired input 
{
	auto& input = dae::InputManager::GetInstance();
	auto moveUp = std::make_shared<MoveCommand>(actor.get(), 50.0f);
	moveUp->SetDirection(glm::vec3(0, -1, 0));
	input.BindCommand(up, dae::InputManager::KeyState::Held, moveUp);

	auto moveRight = std::make_shared<MoveCommand>(actor.get(), 50.0f);
	moveRight->SetDirection(glm::vec3(1, 0, 0));
	input.BindCommand(right, dae::InputManager::KeyState::Held, moveRight);

	auto moveDown = std::make_shared<MoveCommand>(actor.get(), 50.0f);
	moveDown->SetDirection(glm::vec3(0, 1, 0));
	input.BindCommand(down, dae::InputManager::KeyState::Held, moveDown);

	auto moveLeft = std::make_shared<MoveCommand>(actor.get(), 50.0f);
	moveLeft->SetDirection(glm::vec3(-1, 0, 0));
	input.BindCommand(left, dae::InputManager::KeyState::Held, moveLeft);

}

void BindAttackCommmand(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode attackKey)
{
	auto& input = dae::InputManager::GetInstance();
	auto attackCommand = std::make_unique<AttackCommand>(actor.get(), 100.0f, 1.f);
	input.BindCommand(attackKey, dae::InputManager::KeyState::Pressed, std::move(attackCommand));
}

void BindAddPointsCommand(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode addPointsKey)
{
	auto& input = dae::InputManager::GetInstance();
	auto addPointsCommand = std::make_unique<AddPointsCommand>(actor.get(), 100);
	input.BindCommand(addPointsKey, dae::InputManager::KeyState::Pressed, std::move(addPointsCommand));
}

void MakeLivesDisplay(std::shared_ptr<dae::GameObject> actor, std::shared_ptr<dae::GameObject> livesDisplayObj, dae::HealthComponent* healthComponent, dae::Scene* scene)
{
	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto rotLivesText = livesDisplayObj->AddComponent<dae::TextComponent>(livesDisplayObj.get(), std::string("Lives: " + std::to_string(int(healthComponent->GetHealth()))), font2);
	auto livesDisplay = std::make_shared<dae::LivesDisplay>(rotLivesText);
	healthComponent->AttachObserver(livesDisplay);
	scene->Add(livesDisplayObj);
}

void MakePointsDisplay(std::shared_ptr<dae::GameObject> actor, std::shared_ptr<dae::GameObject> pointsDisplayObj, dae::PointsComponent* pointsComponent, dae::Scene* scene)
{
	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto rotPointsText = pointsDisplayObj->AddComponent<dae::TextComponent>(pointsDisplayObj.get(), std::string("Points: " + std::to_string(int(pointsComponent->GetPoints()))), font2);
	auto pointsDisplay = std::make_shared<dae::PointsDisplay>(rotPointsText);
	pointsComponent->AttachObserver(pointsDisplay);
	scene->Add(pointsDisplayObj);
}

void BindSoundTestCommand(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode soundKey, int ID)
{
	auto& input = dae::InputManager::GetInstance();
	auto soundCommand = std::make_shared<SoundCommand>(actor.get(), ID, 1.0f); // Sound ID 1, full volume
	input.BindCommand(soundKey, dae::InputManager::KeyState::Pressed, soundCommand);
}
