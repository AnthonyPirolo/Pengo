#pragma once
#include "BaseComponent.h"

namespace dae
{
	class ImGuiPlotComponent final : BaseComponent
	{
	public:
		//---------------------------------
		//Game Related Functions
		//---------------------------------
		void FixedUpdate(float deltaTime) override;
		void Update() override;
		void LateUpdate() override;
		void Render() const override;

		//---------------------------------
		//Constructor & Destructor
		//---------------------------------
		ImGuiPlotComponent(GameObject* owner, float rotationSpeed, GameObject* target);
		ImGuiPlotComponent(GameObject* owner, float rotationSpeed, const glm::vec3& rotationCenter);
		virtual ~ImGuiPlotComponent() override = default;
		ImGuiPlotComponent(const ImGuiPlotComponent& other) = delete;
		ImGuiPlotComponent(ImGuiPlotComponent&& other) = delete;
		ImGuiPlotComponent& operator=(const ImGuiPlotComponent& other) = delete;
		ImGuiPlotComponent& operator=(ImGuiPlotComponent&& other) = delete;
	private:
		//---------------------------------
		//Plot related variables
		//---------------------------------
		float m_XData[512];
		float m_YData[512];


	};
}

