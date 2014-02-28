#pragma once
#include "Projectile.h"
#include "Arrow.h"
#include "Spell.h"
#include "AOE.h"
#include "Mob\MobManager.h"
#include "Effects\Particles\ParticleSystem.h"
#include "SpellTypeManager.h"

class ProjectileManager : public sf::Drawable{
public:
	ProjectileManager();
	ProjectileManager(MobManager* const& p_mobManager, ParticleSystem* const& p_particleSystem);
	~ProjectileManager();
	std::vector<projectile::Arrow> m_arrows;
	std::vector<projectile::Spell> m_spells;
	std::vector<AOE> m_AOE_spells;

	ParticleSystem* m_particleSystem;

	void update(sf::Time & p_dt, const sf::Rect<float> & p_player_position, int* ptr_player_health);
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;
private:
	MobManager* m_mobManager;
	
};