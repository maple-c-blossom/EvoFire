#include "Collider.h"
#include "Util.h"
using namespace MCB;

bool MCB::CalcSphere(Sphere sphereA, Sphere sphereB)
{

	int hitX = (sphereA.centerPosition.x - sphereB.centerPosition.x) * (sphereA.centerPosition.x - sphereB.centerPosition.x);
	int hitY = (sphereA.centerPosition.y - sphereB.centerPosition.y) * (sphereA.centerPosition.y - sphereB.centerPosition.y);
	int hitZ = (sphereA.centerPosition.z - sphereB.centerPosition.z) * (sphereA.centerPosition.z - sphereB.centerPosition.z);
	int hitR = (sphereB.radius + sphereA.radius) * (sphereB.radius + sphereA.radius);
	hitX = Abs(hitX);
	hitY = Abs(hitY);
	hitZ = Abs(hitZ);
	int hit = hitX + hitY + hitZ;
	if (hit <= hitR)
	{
		return true;
	}
	return false;
}

bool MCB::CalcSphere(Float3 sphereA, float rudiusA, Float3 sphereB, float rudiusB)
{
	int hitX = (sphereB.x - sphereA.x) * (sphereB.x - sphereA.x);
	int hitY = (sphereB.y - sphereA.y) * (sphereB.y - sphereA.y);
	int hitZ = (sphereB.z - sphereA.z) * (sphereB.z - sphereA.z);
	int hitR = (rudiusB + rudiusA) * (rudiusB + rudiusA);
	hitX = Abs(hitX);
	hitY = Abs(hitY);
	hitZ = Abs(hitZ);
	int hit = hitX + hitY + hitZ;
	if (hit <= hitR)
	{
		return true;
	}
	return false;
}

bool MCB::CalcRaySphere(Ray ray, Sphere sphere)
{
	Vector3D rayToSphere(ray.StartPosition, sphere.centerPosition);
	ray.rayVec.V3Norm();
	float tempPos = rayToSphere.GetV3Dot(ray.rayVec);
	ray.rayVec = ray.rayVec * tempPos;
	Float3 Temp;
	Temp.x = ray.rayVec.vec.x + ray.StartPosition.x;
	Temp.y = ray.rayVec.vec.y + ray.StartPosition.y;
	Temp.z = ray.rayVec.vec.z + ray.StartPosition.z;
	//衝突地点がレイの射程を超えているかレイの逆方向だった場合
	if (tempPos > ray.range || tempPos < 0)
	{
		return false;
	}
	Vector3D aVec;

	aVec.V3Get(sphere.centerPosition, Temp);

	float a = aVec.V3Len();

	return a < sphere.radius + ray.radius;
}


