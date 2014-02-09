#include <Mob\Boss.h>	

Boss::Boss() {}

Boss::Boss(TextureHolder* textureHolder, std::vector<DeadMob>* p_deadMobs, std::function<void(void)> die_func) 
{
	playerCollision = false;

	type = TYPE::boss;

	startPos = (sf::Vector2i)getPosition();
	width = GetWidth(type);
	height = GetHeight(type);
	setOrigin(width/2, height/2);

	inventory = Inventory(10, 10, 0, 0, *textureHolder);

	this->p_deadMobs = p_deadMobs;
	this->textureHolder = textureHolder;
	this->die_func = std::move(die_func);
	sprite.setTexture(*textureHolder->getTexture(Textures::Boss));

	timeSincePath = 0.f;
	m_attack_timer = GetAttackSpeed(type);
	worldCollision = false;
	dead = false;
	aggro = false;

	m_current_phase = Phase::Normal;
	m_phase_time = sf::seconds(5);

	m_attack_time = sf::seconds(GetAttackSpeed(type));
	m_attack_range = GetAtackDistance(type);
	m_attack_damage = GetDamage(type);
}

Boss::~Boss() {}

void Boss::update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition, int* p_health, Mob* ptr_mob, std::vector<projectile::Spell>* ptr_spells, std::vector<projectile::Arrow>* ptr_arrows){

	// Check if we should switch phase
	if (m_phase_clock.getElapsedTime() >= m_phase_time){
		m_current_phase = (Phase)math::random(Phase::Normal, Phase::END-1, m_current_phase); 
		m_phase_time = sf::seconds(math::random(5, 10));
		m_phase_clock.restart();
	}

	switch (m_current_phase)
	{
	case Boss::Normal:
		update_normal(deltaTime, playerPosition, p_health);
		break;

	case Boss::Laser:
		update_laser(deltaTime);
		break;

	case Boss::FireCircle:
		update_fireCircle(deltaTime);
		break;

	case Boss::Summon:
		update_summon(deltaTime);
		break;

	default:
		break;
	}
}

void Boss::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(sprite, states);
}

void Boss::takeDamage(int damage) {

}

void Boss::dealDamage(int* health) {
	*health -= m_attack_damage;
}

void Boss::die() {

}

void Boss::update_normal(sf::Time dt,  sf::Vector2f & playerPosition, int* p_health) {
	// if !inrange to attack, move towards player

	if (math::distance(getPosition(), playerPosition) > m_attack_range) {
		followPath(dt);
		move(velocity);
	} else { // attack player
			if (m_attack_clock.getElapsedTime() >= m_attack_time) {
			dealDamage(p_health);
			m_attack_clock.restart();
			// TODO: Attack animation
		}
	}
}

void Boss::update_laser(sf::Time dt) {

	// stand fucking still (some type of root animation)
	// lazer
}

void Boss::update_fireCircle(sf::Time dt) {

	// stand still (some type of root animation)
	// shoot projectiles out in a circle, change the angle for each attack
}

void Boss::update_summon(sf::Time dt) {

	// stand still,
	// summon dudes in a circle around boss

}