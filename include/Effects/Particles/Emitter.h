#pragma once

#include "SFML\Graphics\Drawable.hpp"
#include "SFML\Graphics\Transformable.hpp"
#include "SFML\System\Time.hpp"
#include "SFML\System\Clock.hpp"

#include <vector>

#include "Particle.h"

class Emitter : public sf::Drawable, public sf::Transformable {

	public:
		Emitter(int max_particles, float spawnrate, float emitter_lifetime, bool burst=false);
		~Emitter();

		void setparticle(float max_speed, float lifetime, sf::Color color);
		
		void update(sf::Time dt);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		int id;
		
		float m_elifetime;

	private:
		int m_max_particles;
		float m_spawnrate; // time between spawns in ms
		float m_pmax_speed;
		float m_plifetime;
		sf::Color m_pcolor;
		sf::Texture* m_ptexture;

		bool burst;
		
		std::vector<Particle> m_particles;
		sf::Clock m_spawn_timer;
};