#pragma once
#include <cmath>
#include "Vector3D.h"

namespace MCB
{

	class Ray
	{
	public:
		Float3 StartPosition;
		Vector3D rayVec;
		float range;
		float radius;
	};

	class Box
	{
	public:
		Float3 position;
		Float3 size;
	};

	class Sphere
	{
	public:
		Float3 centerPosition;
		float radius;
	};

	class Plane
	{
	public:
		Vector3D normal = {0,1,0};
		float distance = 0;
	};

	bool CalcRaySphere(Ray ray,Sphere sphere);
	bool CalcRaySphere(Ray ray, Float3 sphereA, float radiusA,float* hitRenge = nullptr);
	bool CalcSphere(Sphere sphereA,Sphere sphereB);
	bool CalcSphere(Float3 sphereA,float radiusA, Float3 sphereB,float rudiusB);
}

