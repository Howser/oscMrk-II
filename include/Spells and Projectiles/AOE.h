#include <SFML\System\Time.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include "Item\ItemManager.h"
#include "Mob\Tree.h"

class AOE : public sf::Transformable, public sf::Drawable{
public:
	AOE();
	AOE(Items p_spell_type, float p_radius, float p_timer, const sf::Vector2f & p_position, mobtree::Tree* ptr_tree);
	~AOE();

	///<summary>An id generated from the player class.</summary>
	int ID;

	float m_radius;
	bool m_dead;
	Items m_spell_type;
	sf::Sprite m_sprite;
	sf::Color m_color;

	///<summary>A vector of branches to check for mobs that are within the radius of the AOE spell.</summary>
	std::vector<mobtree::Branch*> m_branches;

	void update(sf::Time & p_dt);
	void draw(sf::RenderTarget & p_target, sf::RenderStates p_states)const;

private:
	float m_timer;
};