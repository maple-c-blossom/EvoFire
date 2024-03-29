#include "View.h"
#include <DirectXMath.h>

using namespace DirectX;
using namespace MCB;

void View::CreateMatrixView(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up)
{
	this->eye = eye;
	this->target = target;
	this->up = up;
	mat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void View::UpDateMatrixView()
{
	//mat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	XMVECTOR upVector = XMLoadFloat3(&up);

	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	XMVECTOR cameraAxisX;
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	XMVECTOR cameraAxisY;
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	XMMATRIX matCameraRot;
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	mat = XMMatrixTranspose(matCameraRot);

	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);

	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	mat.r[3] = translation;

	billMat.r[0] = cameraAxisX;
	billMat.r[1] = cameraAxisY;
	billMat.r[2] = cameraAxisZ;
	billMat.r[3] = XMVectorSet(0, 0, 0, 1);
}

void MCB::View::FollowingFor3DObject(DirectX::XMFLOAT3 position, Vector3D frontVec, Float3 distance, Vector3D UpVec)
{
	

	eye.x = position.x + (-frontVec.vec.x * distance.x);
	eye.y = position.y + (-frontVec.vec.y * distance.y) + 30;
	eye.z = position.z + (-frontVec.vec.z * distance.z);

	target.x = position.x;
	target.y = position.y;
	target.z = position.z;

	up.x = UpVec.vec.x;
	up.y = UpVec.vec.y;
	up.z = UpVec.vec.z;

}
