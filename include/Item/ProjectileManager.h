#pragma once
#include "Arrow.h"

class ProjectileManager : public sf::Drawable{
public:
	ProjectileManager();
	~ProjectileManager();

	std::vector<projectile::Arrow> m_arrows;

	void update();
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;
};