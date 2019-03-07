#pragma once
#include "singletonBase.h"

class camera : public singletonBase<camera>
{
private:
	POINT _mapSize;

	POINTFLOAT _pos;

	POINTFLOAT _playerPos;

	int _shakeCount;
	int _shakeValue;

private:
	const int CAMERA_SPEED = 5;
public:
	camera();
	~camera();

	HRESULT init();
	void release();
	void update();

	POINTFLOAT getPos() { return _pos; }
	float getPosX() { return _pos.x; }
	float getPosY() { return _pos.y; }
	int getCameraSpeed() { return CAMERA_SPEED; }

	void setPosX(float posX) { _pos.x = posX; }
	void setPosY(float posY) { _pos.y = posY; }
	void setPos(POINTFLOAT playerPos);	//이거 어케쓰는건지 물어볼것.
	void setMapSize(int mapSizeX, int mapSizeY) { _mapSize.x = mapSizeX, _mapSize.y = mapSizeY; }
	void setShake(int shakeValue, int shakeCount) { _shakeValue = shakeValue, _shakeCount = shakeCount; }



};

