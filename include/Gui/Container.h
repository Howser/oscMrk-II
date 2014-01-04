/* 
	Gui container,
	Can hold any gui object that derives from Component
	Passes on events to the selected child if it is active
	A Container cannot be selected
	
	--==-- Might move the input code out of the container, not sure yet --==--
*/

#include "Gui\Component.h"

#include <vector>


namespace gui
{
	class Container : public Component
	{
		public:
							Container();
			virtual			~Container();

			virtual bool	isSelectable() const;// Returns false

			virtual void 	handleEvent(sf::Event const& event);

			void			add(Component::ptr component);

		private:
			void			selectNext();
			void			selectPrevious();

			virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;
			
		private:
			std::vector<ptr>	mChildren;
			size_t				mSelectedChild;
	};
}