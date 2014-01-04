#include <SFML\Graphics\Transformable.hpp>
#include <SFML\System\Vector2.hpp>

namespace gen{
struct HealingWell : sf::Transformable{
		HealingWell(sf::Vector2f position);

		int charges;
		
		void use();
	};
}