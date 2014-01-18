#include "Effects\Particles\Emitter.h"
#include "Math\GeneralMath.h"

#include "SFML\Graphics\CircleShape.hpp"
#include "SFML\Graphics\RenderTarget.hpp"
#include "SFML\Graphics\RenderStates.hpp"

#include <iostream>

Emitter::Emitter(int max_particles, float spawnrate, float emitter_lifetime, sf::Sprite & p_sprite, bool burst)
	:
	m_max_particles(max_particles),
	m_spawnrate(spawnrate),
	m_particles(max_particles),
	m_elifetime(emitter_lifetime),
	m_sprite(p_sprite),
	burst(burst)
{
}

Emitter::Emitter(){
	m_max_particles = 0;
	m_spawnrate = 0;
	m_particles.clear();
	m_elifetime = 0;
	burst = false;
}

Emitter::~Emitter(){}

void Emitter::setparticle(float max_speed, float lifetime, sf::Color color){
	m_pmax_speed = max_speed;
	m_plifetime = lifetime;
	m_pcolor = color;
}

void Emitter::update(sf::Time dt){
	bool canspawn = false;
	if (m_spawn_timer.getElapsedTime().asMilliseconds() > m_spawnrate && m_particles.size() < m_max_particles){
		canspawn = true;
		m_spawn_timer.restart();
	}

	if (m_elifetime > 0)
	{
		if (m_elifetime - dt.asMilliseconds() != -1)
		{
			m_elifetime -= dt.asMilliseconds();
		}else
		{
			m_elifetime -= dt.asMilliseconds() + 1;
		}
	}
	m_sprite.setRotation(m_sprite.getRotation() + 1.f);
	if (burst){
		// find 20 dead particles and spawn them 
		int count = 0;
		for (auto i = m_particles.begin(); i != m_particles.end(); ++i){
			if (i->dead){
				i->lifetime = m_plifetime;
				i->color = m_pcolor;
				i->velocity.x = ((float)math::random(-100, 100) / 100.f) * m_pmax_speed;
				i->velocity.y = ((float)math::random(-100, 100) / 100.f) * m_pmax_speed;
				i->setPosition(getPosition());
				i->dead = false;
				count++;
				if (count == 20)
					break;
			}
		}
		burst = false;
	}

	for (auto i = m_particles.begin(); i != m_particles.end(); ++i){
		if (!i->dead){
			i->lifetime -= dt.asMilliseconds();
			if (i->lifetime < 0)
				i->dead = true;
			i->move(i->velocity);
			i->color.a = i->lifetime / m_plifetime * 255;
		} else if (canspawn && !burst){
			canspawn = false;

			i->lifetime = m_plifetime;
			i->color = m_pcolor;
			i->velocity.x = ((float)math::random(-100, 100) / 100.f) * m_pmax_speed;
			i->velocity.y = ((float)math::random(-100, 100) / 100.f) * m_pmax_speed;
			i->setPosition(getPosition());
			i->dead = false;
		}
	}
}

void Emitter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto i = m_particles.begin(); i != m_particles.end(); ++i){
		if (!i->dead){
			sf::Sprite sprite = m_sprite;
			sprite.setPosition(i->getPosition());
			target.draw(sprite);
		}
	}
}