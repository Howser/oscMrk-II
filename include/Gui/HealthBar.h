#include "StatusBar.h"
#include "Animation\Animation.h"

namespace gui
{
	class HealthBar : public gui::StatusBar
	{
		public:
			HealthBar(sf::Texture const& base, sf::Texture const& loaded);
			virtual ~HealthBar();

			void updateStatus(float x);
			void updateStatus(float current, float max);

			void update();

			void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		private:
			Animation m_animation;

	};
}