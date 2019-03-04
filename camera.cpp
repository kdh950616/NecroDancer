#include "stdafx.h"
#include "camera.h"


camera::camera()
{
}


camera::~camera()
{
}

HRESULT camera::init()
{
	_pos.x = 0;
	_pos.y = 0;

	_playerPos.x = 0;
	_playerPos.y = 0;

	_shakeValue = 0;
	_shakeCount = 0;

	return S_OK;
}

void camera::release()
{
}

void camera::update()
{
	_pos.x = _playerPos.x;
	_pos.y = _playerPos.y;

	//흔들림 -> shakeValue는 옵션에서 설정하도록.
	if (_shakeCount > 0)
	{
		_shakeCount--;
		POINT shake = { RND->getFromIntTo(-1, 2) ,RND->getFromIntTo(-1, 2) };
		_pos.x += shake.x * _shakeValue;
		_pos.y += shake.y * _shakeValue;
	}
}

void camera::setPos(POINTFLOAT playerPos)
{
	if (playerPos.x > WINSIZEX / 2 && playerPos.x < _mapSize.x - WINSIZEX / 2)
	{
		_playerPos.x = playerPos.x - WINSIZEX / 2;
	}
	else if (playerPos.x <= WINSIZEX / 2)
	{
		_playerPos.x = 0;
	}

	if (playerPos.y > WINSIZEY / 2 && playerPos.y < _mapSize.y - WINSIZEY / 2)
	{
		_playerPos.y = playerPos.y - WINSIZEY / 2;
	}
	else if (playerPos.y <= WINSIZEY / 2)
	{
		_playerPos.y = 0;
	}
}