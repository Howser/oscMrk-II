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

	health = GetHp(type);

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
		update_normal(deltaTime, playerPosition, p_health, map);
		break;

	case Boss::Laser:
		update_laser(deltaTime, playerPosition, ptr_spells, map);
		break;

	case Boss::FireCircle:
		update_fireCircle(deltaTime, ptr_spells, map);
		break;

	case Boss::Summon:
		update_summon(deltaTime);
		break;

	default:
		break;
	}

	for (int i = 0; i < m_buffs.size(); i++)
	{
		if (m_buffs[i].m_duration > 0)
		{
			m_buffs[i].update(deltaTime);
		}else
		{
			m_buffs.erase(m_buffs.begin() + i);
		}
	}
}

void Boss::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(sprite, states);
}

void Boss::takeDamage(int damage) {
	health -= damage;
	if (health <= 0) {
		die();
	} else {
		aggro = true;
	}
	std::cout << "Boss HP: " << health << std::endl;
}

void Boss::dealDamage(int* health) {
	*health -= m_attack_damage;
}

void Boss::die() {
	die_func();
}

void Boss::update_normal(sf::Time dt,  sf::Vector2f & playerPosition, int* p_health, std::vector<std::vector<gen::Tile>>* map) {
	if (!dead) {
		// if !inrange to attack, move towards player
		m_attack_time = sf::seconds(GetAttackSpeed(type));

		if (math::distance(getPosition(), playerPosition) > m_attack_range) {
			followPath(dt);
			move(velocity);
			checkCollision(map, playerPosition);
		} else { // attack player
			if (m_attack_clock.getElapsedTime() >= m_attack_time) {
				dealDamage(p_health);
				m_attack_clock.restart();
				// TODO: Attack animation
			}
		}
	}
}

void Boss::update_laser(sf::Time dt, sf::Vector2f& playerPosition, std::vector<projectile::Spell>* ptr_spells,  std::vector<std::vector<gen::Tile>>* map) {

	// setup stuff
	m_attack_time = sf::seconds(0.1f);
	float spell_speed = 5;

	// stand fucking still (some type of root animation)
	stop();
	// lazer
	if (m_attack_clock.getElapsedTime() >= m_attack_time) {
		float angle = std::atan2f(playerPosition.y - getPosition().y, playerPosition.x - getPosition().x);
		sf::Sprite spell_sprite;
		spell_sprite.setTexture(*textureHolder->getTexture(Textures::Armor_Chaos));
		ptr_spells->push_back(projectile::Spell(getPosition(), angle, Items::TestSpell, spell_sprite, map, true));
		m_attack_clock.restart();
	}
}

void Boss::update_fireCircle(sf::Time dt, std::vector<projectile::Spell>* ptr_spells,  std::vector<std::vector<gen::Tile>>* map) {
	// stand still (some type of root animation)
	stop();

	// setup stuff
	static int angle = 0;
	int projectile_nr = 8;
	int diff_angle = 360 / projectile_nr;
	m_attack_time = sf::seconds(0.3f);

	// shoot projectiles out in a circle, change the angle for each attack
	if (m_attack_clock.getElapsedTime() >= m_attack_time){

		for (int i = 0; i < projectile_nr; i++) {

			sf::Sprite spell_sprite;
			spell_sprite.setTexture(*textureHolder->getTexture(Textures::Armor_Chaos));
			ptr_spells->push_back(projectile::Spell(getPosition(), math::toRadians(angle + diff_angle*i), Items::TestSpell, spell_sprite, map, true));
		}
		angle += 10;
		m_attack_clock.restart();
	}
}

void Boss::update_summon(sf::Time dt) {

	// stand still,
	// summon dudes in a circle around boss

}