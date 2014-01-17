#include "Effects\Particles\ParticleSystem.h"
#include "SFML\Graphics\RenderTarget.hpp"
#include "SFML\Graphics\RenderStates.hpp"

#include <iostream>

ParticleSystem::ParticleSystem(){
	m_cid = 0;
}
ParticleSystem::~ParticleSystem(){}

int ParticleSystem::add(Emitter & e){
	e.id = m_cid;
	m_emitters.push_back(e);
	return m_cid++;
}

void ParticleSystem::remove(int id){
	for (auto i = m_emitters.begin(); i != m_emitters.end(); ++i){
		 if (i->id == id){
			m_emitters.erase(i);
			break;
		 }
	}
}

Emitter* ParticleSystem::getEmitter(int id){
	for (auto i = m_emitters.begin(); i != m_emitters.end(); ++i){
		if (i->id == id){
			return &(*i);
		}
	}
}

void ParticleSystem::update(sf::Time & dt){
	std::vector<int> rm_emitters;
	for (auto i = m_emitters.begin(); i != m_emitters.end(); ++i){
		i->update(dt);
		if (i->m_elifetime < 0){
			rm_emitters.push_back(i->id);
		}
	}
	for (auto i = rm_emitters.begin(); i != rm_emitters.end(); ++i){
		remove(*i);
	}
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	for (auto i = m_emitters.begin(); i != m_emitters.end(); ++i){
		target.draw(*i);
	}
}