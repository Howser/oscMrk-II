#pragma once

#include "SFML\Graphics\Drawable.hpp"

#include <vector>

#include "Emitter.h"


class ParticleSystem : public sf::Drawable{
	public:
		ParticleSystem();
		~ParticleSystem();

		int add(Emitter e);
		void remove(int id);
		Emitter* getEmitter(int id);

		void update(sf::Time dt);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	private:
		std::vector<Emitter> m_emitters;
		int m_cid;
};