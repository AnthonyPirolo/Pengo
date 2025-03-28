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

void BindMovementCommands(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode up, SDL_KeyCode right, SDL_KeyCode left, SDL_KeyCode down);
void BindAttackCommmand(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode attackKey);
void BindAddPointsCommand(std::shared_ptr<dae::GameObject> actor, SDL_KeyCode addPointsKey);
void MakeLivesDisplay(std::shared_ptr<dae::GameObject> actor, std::shared_ptr<dae::GameObject> livesDisplayObj, dae::HealthComponent* healthComponent, dae::Scene* scene);
void MakePointsDisplay(std::shared_ptr<dae::GameObject> actor, std::shared_ptr<dae::GameObject> pointsDisplayObj, dae::PointsComponent* pointsComponent, dae::Scene* scene);


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// W01 - W02
	auto background = std::make_shared<dae::GameObject>();
	background->AddComponent<dae::TextureComponent>(background.get(), "background.tga");
	scene.Add(background);

	auto logo = std::make_shared<dae::GameObject>();
	logo->AddComponent<dae::TextureComponent>(logo.get(), "logo.tga");
	logo->SetLocalPosition(glm::vec3(216, 180, 0));
	scene.Add(logo);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto text = std::make_shared<dae::GameObject>();
	text->AddComponent<dae::TextComponent>(text.get(), "Programming 4 Assignment", font);
	text->AddComponent<dae::TextComponent>(text.get(), "Deletion", font);
	text->SetLocalPosition(glm::vec3(180, 20, 0));
	scene.Add(text);

	auto fps = std::make_shared<dae::GameObject>();
	fps->AddComponent<dae::TextComponent>(fps.get(), "FPS: 0", font);
	fps->AddComponent<dae::FpsComponent>(fps.get());
	fps->SetLocalPosition(glm::vec3(10, 20, 0));
	scene.Add(fps);

	auto rot = std::make_shared<dae::GameObject>();
	rot->AddComponent<dae::TextureComponent>(rot.get(), "SinglePenguin.png");
	rot->SetLocalPosition(glm::vec3(500, 300, 0));
	scene.Add(rot);
	auto rot1Health = rot->AddComponent<dae::HealthComponent>(rot.get());
	rot1Health->SetHealth(3);
	auto rotPoints = rot->AddComponent<dae::PointsComponent>(rot.get());
	//rot->AddComponent<dae::RotationComponent>(rot.get(), 360.f , glm::vec3(300, 300, 0));

	BindMovementCommands(rot, SDLK_w, SDLK_d, SDLK_a, SDLK_s);
	BindAttackCommmand(rot, SDLK_SPACE);
	BindAddPointsCommand(rot, SDLK_p);


	auto rot2 = std::make_shared<dae::GameObject>();
	rot2->SetLocalPosition(glm::vec3(50, 300, 0));
	rot2->AddComponent<dae::TextureComponent>(rot2.get(), "SinglePenguin2.png");
	scene.Add(rot2);
	auto rot2Health = rot2->AddComponent<dae::HealthComponent>(rot2.get());
	rot2Health->SetHealth(3);
	auto rot2Points = rot2->AddComponent<dae::PointsComponent>(rot2.get());
	//rot2->AddComponent<dae::RotationComponent>(rot2.get(), -180.f, rot.get());

	BindMovementCommands(rot2, SDLK_UP, SDLK_RIGHT, SDLK_LEFT, SDLK_DOWN);
	BindAttackCommmand(rot2, SDLK_RETURN);
	BindAddPointsCommand(rot2, SDLK_o);

	auto font2 = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto livesDisplayObj1 = std::make_shared<dae::GameObject>();
	livesDisplayObj1->SetLocalPosition(glm::vec3(50, 200, 0));
	MakeLivesDisplay(rot, livesDisplayObj1, rot1Health, &scene);

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
	MakePointsDisplay(rot, pointsDisplayObj1, rotPoints, &scene);

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