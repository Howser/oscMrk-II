#include "StatusBar.h"

namespace gui
{
	class HealthBar : public gui::StatusBar
	{
		public:
			HealthBar(sf::Texture const& base, sf::Texture const& loaded);
			virtual ~HealthBar();

			void updateStatus(float x);
			void updateStatus(float current, float max);
	};
}