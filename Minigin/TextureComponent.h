#pragma once
#include <string>
#include <memory>
#include "BaseComponent.h"
#include "Transform.h"
#include "GameObject.h"

namespace dae
{
	class Texture2D;
	class TextureComponent final : public BaseComponent
	{
	public:
		//---------------------------------
		//Game Related Functions
		//---------------------------------
		void FixedUpdate(float DeltaTime) override;
		void Update() override;
		void LateUpdate() override;
		void Render() const override;

		//---------------------------------
		//Texture Related Functions
		//---------------------------------
		void SetTexture(const std::string& filename);

		//---------------------------------
		//Constructor & Destructor
		//---------------------------------
		TextureComponent(GameObject* owner, const std::string& path);
		virtual ~TextureComponent() override = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;
	private:
		//---------------------------------
		//Texture Related Variables
		//---------------------------------
		bool m_needsUpdate;
		std::shared_ptr<Texture2D> m_texture;
	};
}
