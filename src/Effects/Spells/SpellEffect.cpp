#include "Effects\Spells\SpellEffect.h"

SpellEffect::SpellEffect(Light* ptr_light, Emitter & p_emitter, sf::Vector2<float>* ptr_position) : ptr_light(ptr_light), m_emitter(p_emitter), ptr_position(ptr_position){
	
}

SpellEffect::SpellEffect() : ptr_light(NULL), m_emitter(Emitter()){}

SpellEffect::~SpellEffect(){}

void SpellEffect::update(sf::Time & p_dt){
	m_emitter.update(p_dt);
	m_emitter.setPosition(*ptr_position);
}