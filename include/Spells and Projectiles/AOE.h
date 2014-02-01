#include <SFML\System\Time.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include "Item\ItemManager.h"
#include "Spells and Projectiles\Buff.h"

class AOE : public sf::Transformable, public sf::Drawable{
public:
	AOE();
	AOE(Items & p_spell_type, float & p_radius, float & p_timer, sf::Vector2<int> & p_position);
	~AOE();

	///<summary>An id generated from the player class.</summary>
	int ID;

	float m_radius;
	bool m_dead;
	Items m_spell_type;
	Buff m_buff;

	void update(sf::Time & p_dt);
	void draw(sf::RenderTarget & p_target, sf::RenderStates p_states)const;

private:
	float m_timer;
	sf::Sprite m_sprite;
};