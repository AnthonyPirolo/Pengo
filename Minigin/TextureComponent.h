#pragma once
#include <string>
#include <memory>
#include "BaseComponent.h"
#include "Transform.h"

namespace dae
{
	class Texture2D;
	class TextureComponent final : public BaseComponent
	{
	public:
		void Update(float DeltaTime) override;
		void Render() const override;

		void SetPosition(float x, float y) override;

		TextureComponent(const std::string& path);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;
	private:
		bool m_needsUpdate;
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture;
	};
}
