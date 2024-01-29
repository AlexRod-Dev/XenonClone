#include <SDL.h>
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "Texture.h"
#include "Animation.h"
#include "GameEngine.h"
#include "LogOutput.h"

SpriteComponent::SpriteComponent(const char* texPath)
{
	SetTexture(texPath);
	m_flashTimer = false;
	m_whiteTexture = SDL_CreateTexture(GameEngine::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 64);
	SDL_SetTextureColorMod(m_whiteTexture, 511, 511, 511);
}

SpriteComponent::SpriteComponent(const char* texPath, bool isAnimated, bool isLoop)
{
	m_animated = isAnimated;
	m_loopable = isLoop;
	SetTexture(texPath);
}

SpriteComponent::~SpriteComponent()
{
	SDL_DestroyTexture(m_texture);
}

void SpriteComponent::Init()
{
	m_transformComponent = &entity->GetComponent<TransformComponent>();

	m_srcRect.x = m_srcRect.y = 0;
	m_srcRect.w = m_transformComponent->width;
	m_srcRect.h = m_transformComponent->height;
}

void SpriteComponent::Update()
{
	if (m_animated && m_loopable)
	{
		m_srcRect.x = m_srcRect.w * static_cast<int>((SDL_GetTicks() / m_speed) % m_frames);
	}
	else if (m_animated && !m_loopable)
	{
		m_srcRect.x = m_srcRect.w * static_cast<int>((SDL_GetTicks() / m_speed));
	}

	m_srcRect.y = m_animIndex * m_transformComponent->height;

	m_dstRect.x = static_cast<int>(m_transformComponent->GetPosition().X());
	m_dstRect.y = static_cast<int>(m_transformComponent->GetPosition().Y());
	m_dstRect.w = m_transformComponent->width * 1;
	m_dstRect.h = m_transformComponent->height * 1;
}

void SpriteComponent::Draw()
{
	if (m_flashing)
	{
		SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_ADD);
		SDL_SetTextureBlendMode(m_whiteTexture, SDL_BLENDMODE_MOD);

		Texture::Draw(m_whiteTexture, m_srcRect, m_dstRect, spriteFlip);
	}
	else
	{
		SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
		Texture::Draw(m_texture, m_srcRect, m_dstRect, spriteFlip);
	}
}

void SpriteComponent::CreateAnimation(const char* animName, int startFrame, int endFrame, int frameRate)
{
	m_animations.emplace(animName, Animation(startFrame, endFrame, frameRate));
}

void SpriteComponent::PlayAnimation(const char* animName)
{
	if (m_animations.find(animName) != m_animations.end())
	{
		m_frames = m_animations[animName].m_frames;
		m_animIndex = m_animations[animName].m_index;
		m_speed = m_animations[animName].m_speed;
	}
	else
	{
		DebugLog(LogMessage::WARNING, "Animation " + std::string(animName) + " was not found!");
	}
}

void SpriteComponent::SetTexture(const char* texPath)
{
	m_texture = Texture::LoadTexture(texPath);
}

void SpriteComponent::SetFlashing(bool flashing)
{
	m_flashing = flashing;
	m_flashTimer = 0;
}
