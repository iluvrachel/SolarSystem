#pragma once
#ifndef PLANET_H
#define PLANET_H

class Planet
{
public:
	float srot;
	float arot;
	float radius;
	GLuint texture_name;
	float distance;

	float counter_a = 0.0f;
	float counter_s = 0.0f;
public:
	Planet(float sr, float ar, float r, GLuint tex, float dis)
	{
		this->srot = sr;
		this->arot = ar;
		this->radius = r;
		this->texture_name = tex;
		this->distance = dis;
	}

	~Planet()
	{

	}
};

#endif