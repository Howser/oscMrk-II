#pragma once
#include "Projectile.h"
#include "Arrow.h"
#include "Spell.h"
#include "Mob\MobManager.h"

class ProjectileManager : public sf::Drawable{
public:
	ProjectileManager::ProjectileManager();
	ProjectileManager(MobManager* const& p_mobManager);
	~ProjectileManager();

	typedef std::vector<projectile::Projectile*> Projectiles;
	Projectiles m_projectiles;
	std::vector<projectile::Arrow> m_arrows;
	std::vector<projectile::Spell> m_spells;

	void update();
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;
private:
	MobManager* m_mobManager;
};