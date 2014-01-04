#pragma once

#include <map>
#include <memory>
#include <string>

#include <SFML\Graphics\Texture.hpp>
#include "ResourceIdentifiers.h"

class TextureHolder
{
	public:
											TextureHolder();
		virtual								~TextureHolder();

		void								loadTexture(Textures::ID id, const std::string& path);
		sf::Texture*						getTexture(Textures::ID id) const;


	private:
		std::map<Textures::ID, 
			std::unique_ptr<sf::Texture>>	mTextureMap;
};