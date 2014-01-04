#include "ResourceHolders\FontHolder.h"

#include <stdexcept>

FontHolder::FontHolder()
	:
	mFontMap()
{
}

FontHolder::~FontHolder()
{
}

void FontHolder::loadFont(Fonts::ID id, const std::string path)
{
	std::unique_ptr<sf::Font> font(new sf::Font());
	if (!font->loadFromFile(path))
	{
		throw std::runtime_error("Couldn't load resource: " + path);
	}
	mFontMap.insert(std::make_pair(id, std::move(font)));
}

sf::Font* FontHolder::getFont(Fonts::ID id) const
{
	return mFontMap.at(id).get();
}