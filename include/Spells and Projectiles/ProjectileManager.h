#pragma once
#include "Projectile.h"
#include "Arrow.h"
#include "Spell.h"
#include "AOE.h"
#include "Mob\MobManager.h"
#include "Effects\Particles\ParticleSystem.h"
#include "SpellTypeManager.h"

struct PlayerValues {
	int* ptr_health;
	float* ptr_armor;
	sf::Rect<int> m_position;
};

class ProjectileManager : public sf::Drawable{
public:
	ProjectileManager();
	ProjectileManager(MobManager* const& p_mobManager, ParticleSystem* const& p_particleSystem);
	~ProjectileManager();
	std::vector<projectile::Arrow> m_arrows;
	std::vector<projectile::Spell> m_spells;
	std::vector<AOE> m_AOE_spells;

	ParticleSystem* m_particleSystem;

	void update(sf::Time & p_dt, const PlayerValues & p_player_values);
	void draw(sf::RenderTarget & target, sf::RenderStates states)const;
private:
	MobManager* m_mobManager;
	
};