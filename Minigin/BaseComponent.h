#pragma once
#include <memory>
#include "Transform.h"

class BaseComponent
{
public:
	virtual void Update(float deltaTime) = 0;
	virtual void Render() const = 0;

	virtual void SetPosition(float x, float y) = 0;

	BaseComponent() = default;
	virtual ~BaseComponent() = default;
	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) = delete;
};

