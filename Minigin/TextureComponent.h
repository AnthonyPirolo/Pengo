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
		void FixedUpdate(float DeltaTime) override;
		void Update() override;
		void LateUpdate() override;
		void Render() const override;

		void SetTexture(const std::string& filename);

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
