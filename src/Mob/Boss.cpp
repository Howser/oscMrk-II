#include <Mob\Boss.h>	

Boss::Boss() {}

Boss::Boss(TextureHolder* textureHolder, std::vector<DeadMob>* p_deadMobs, std::function<void(void)> die_func) 
{
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

	m_phase_timer = sf::seconds(5);
}


// THIS SHOULDN'T BE USED IT DOESN'T WORK
Boss::Boss(Mob const& mob, std::vector<DeadMob>* p_deadMobs) {	
	type = TYPE::boss;

	startPos = (sf::Vector2i)getPosition();
	width = GetWidth(type);
	height = GetHeight(type);
	setOrigin(width/2, height/2);
	
	inventory = Inventory(10, 10, 0, 0, *textureHolder);

	this->p_deadMobs = p_deadMobs;
	this->textureHolder = mob.textureHolder;
	this->die_func = std::move(die_func);
	sprite.setTexture(*textureHolder->getTexture(Textures::Boss));
}

Boss::~Boss() {}

void Boss::update(std::vector<std::vector<gen::Tile>>* map, sf::Time& deltaTime, sf::Vector2f & playerPosition, int* p_health){

	if (m_phase_clock.getElapsedTime() >= m_phase_time){
		// math::random(Phase::Normal, Phase::END-1, m_
	}
}

void Boss::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(sprite, states);
}

void Boss::takeDamage(int damage) {
	
}

void Boss::dealDamage(int* health) {
	
}

void Boss::die() {

}