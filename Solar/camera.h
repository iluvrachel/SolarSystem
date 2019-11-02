#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <windows.h>
#include "Angel.h"
#include "mat.h"
#include "vec.h"
#include<vector>
#pragma comment(lib, "glew32.lib")

class Camera
{
public:
	void view1(float radius,float rotateAngle,float upAngle);
	void view2();

	/** ��ת��������� */
	void rotateView(float angle, float x, float y, float z);

	/** �����������������۲췽�� */
	void setViewByMouse(int x,int y);

	/** ����������ƶ� */
	void yawCamera(float speed);

	/** ǰ���ƶ������ */
	void moveCamera(float speed);

	vec3 eye = vec3{ 0.0,0.0,1000.0 };
	vec3 at = vec3{ 0.0,0.0,0.0 };
	vec3 up = vec3{ 0.0,1.0,0.0 };

};

#endif