#pragma once
#include "Arrow.h"
#include "Mob\MobManager.h"

class ProjectileManager : public sf::Drawable{
public:
	ProjectileManager::ProjectileManager();
	ProjectileManager(MobManager* const& p_mobManager);
	~ProjectileManager();

	std::vector<projectile::Arrow> m_arrows;

	void update();
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;

private:
	MobManager* m_mobManager;
};