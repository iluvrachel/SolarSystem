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

	/** 旋转摄像机方向 */
	void rotateView(float angle, float x, float y, float z);

	/** 根据鼠标设置摄像机观察方向 */
	void setViewByMouse(int x,int y);

	/** 左右摄像机移动 */
	void yawCamera(float speed);

	/** 前后移动摄像机 */
	void moveCamera(float speed);

	vec3 eye = vec3{ 0.0,0.0,1000.0 };
	vec3 at = vec3{ 0.0,0.0,0.0 };
	vec3 up = vec3{ 0.0,1.0,0.0 };

};

#endif