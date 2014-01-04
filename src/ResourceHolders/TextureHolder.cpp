#include "ResourceHolders\TextureHolder.h"
#include <iostream>
#include <stdexcept>

TextureHolder::TextureHolder()
	: 
	mTextureMap()
{
}

TextureHolder::~TextureHolder()
{
}

void TextureHolder::loadTexture(Textures::ID id, const std::string& path)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	if (!texture->loadFromFile(path))
	{
		throw std::runtime_error("Couldn't load resources: " + path);
	}
	mTextureMap.insert(std::make_pair(id, std::move(texture)));
}

sf::Texture* TextureHolder::getTexture(Textures::ID id) const
{
	return mTextureMap.at(id).get();
}