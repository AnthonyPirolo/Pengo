#pragma once
#include "BaseComponent.h"

namespace dae
{
	class PointsComponent final : public BaseComponent
	{
	public:
		//---------------------------------
		//Constructor & Destructor
		//---------------------------------
		PointsComponent(GameObject* owner);
		virtual ~PointsComponent() override = default;

		virtual void FixedUpdate(float deltaTime) override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() const override;

		void AddPoints();
		int GetPoints();

		PointsComponent(const PointsComponent& other) = delete;
		PointsComponent(PointsComponent&& other) = delete;
		PointsComponent& operator=(const PointsComponent& other) = delete;
		PointsComponent& operator=(PointsComponent&& other) = delete;

	private:
		int m_Points;
		virtual void Notify(Observer::Event event) const;
	};
}
