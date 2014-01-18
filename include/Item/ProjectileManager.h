#pragma once
#include "Projectile.h"
#include "Arrow.h"
#include "Spell.h"
#include "Mob\MobManager.h"
#include "Effects\Particles\ParticleSystem.h"

class ProjectileManager : public sf::Drawable{
public:
	ProjectileManager::ProjectileManager();
	ProjectileManager(MobManager* const& p_mobManager, ParticleSystem* const& p_particleSystem);
	~ProjectileManager();

	typedef std::vector<projectile::Projectile*> Projectiles;
	Projectiles m_projectiles;
	std::vector<projectile::Arrow> m_arrows;
	std::vector<projectile::Spell> m_spells;

	ParticleSystem* m_particleSystem;

	void update(sf::Time & p_dt);
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;
private:
	MobManager* m_mobManager;
	
};