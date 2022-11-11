#pragma once

#include "ObjectBase.h"

class Particle : public ObjectBase
{
public:
	Particle();
	virtual ~Particle();

	virtual void init() override;
	virtual void end() override;

	virtual void update() override;
	virtual void draw() override;

	void start(Vec2 pos, int color);

private:
	Vec2	m_vec;
	int		m_color;
};
