#pragma once

#include <map>
#include <memory>
#include <string>

#include <SFML\Graphics\Font.hpp>
#include "ResourceIdentifiers.h"

class FontHolder
{
	public:
										FontHolder();
		virtual							~FontHolder();

		void							loadFont(Fonts::ID id, const std::string path);
		sf::Font*						getFont(Fonts::ID id) const;

	private:
		std::map<Fonts::ID, 
			std::unique_ptr<sf::Font>>	mFontMap;
};