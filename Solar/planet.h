#pragma once
#ifndef PLANET_H
#define PLANET_H

class Planet
{
public:
	float srot;
	float arot;
	float radius;
	int texture_name;
	float distance;
public:
	Planet(float sr, float ar, float r, int tex, float dis)
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