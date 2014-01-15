#include <SFML\System\Vector2.hpp>
#include "Math\GeneralMath.h"

namespace gen{
struct Cell{
		Cell(int x, int y);

		int x, y;
		bool active;

		sf::Vector2i digpos();
		void dig(sf::Vector2i & pos);
	};
}