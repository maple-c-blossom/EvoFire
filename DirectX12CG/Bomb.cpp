#include "Bomb.h"
#include "Util.h"

using namespace MCB;
void Bomb::Update()
{
	if (rudiusUpMaxTime > rudiusUpTime)
	{
		rudiusUpTime++;
		position.x += velosity.vec.x * speedOffset;
		position.y += velosity.vec.y * speedOffset;
		position.z += velosity.vec.z * speedOffset;
	}
	if (lifeTime >= maxLifeTime - 20)
	{
		rudiusUpTime -= rudiusUpMaxTime / 20;
		if (rudiusUpTime < 0)
		{
			rudiusUpTime = 0;
		}
	}
		rotasion.x += 0.025f;
		rotasion.y += 0.025f;
		rotasion.z += 0.025f;
	r = OutQuad(0, maxRudius, rudiusUpMaxTime, rudiusUpTime);
	if (rudiusUpMaxTime / 2 > rudiusUpTime)
	{
		speedOffset = InQuad(0, 20, rudiusUpMaxTime / 2, rudiusUpTime);
	}
	if (rudiusUpMaxTime > rudiusUpTime)
	{
		position.x += velosity.vec.x * speedOffset;
		position.y += velosity.vec.y * speedOffset;
		position.z += velosity.vec.z * speedOffset;
	}
	lifeTime++;
	if (lifeTime > maxLifeTime)
	{
		deleteFlag = true;
	}
	scale = { r,r,r };

}

void Bomb::Fire(MCB::Vector3D velosity, MCB::Float3 startPosition, MCB::Model* bombModel)
{
	position.x = startPosition.x;
	position.y = startPosition.y;
	position.z = startPosition.z;
	model = bombModel;
	lifeTime = 0;
	deleteFlag = false;
	maxRudius = 500.0f;
	rudiusUpTime = 0;
	r = 0;
	this->velosity = velosity;
}
