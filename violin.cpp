#include "stdafx.h"
#include "violin.h"
#include "player.h"


violin::violin()
{
}


violin::~violin()
{
}

HRESULT violin::init()
{
	imageInit();

	_maxHp = 3;
	_curHp = _maxHp;
	_speed = SPEED;
	_dmg = 3;
	_posZ = 0;
	_enemyType = VIOLIN;
	_isNeedAstar = true;
	_savePos = _posLT;

	if (_idx.x < 6)
	{
		_isLeft = true;
	}
	else _isLeft = false;

	_moveCount = 23;
	_shakeVal = 0;

	return S_OK;
}

void violin::release()
{
}

void violin::update()
{
	if (_isBeat)
	{
		_isBeat = false;
		_beatCount++;
		if (_beatCount >= 53)//여기추가
		{
			_isFind = true;
			_isNeedAstar = true;
			_beatCount = 0;
		}
		if (_moveCount <= _beatCount && !_isFind)
		{
			if (_moveCount == _beatCount)
			{
				_shakeVal = 3;
			}
			else if (_moveCount == _beatCount - 1 || _moveCount == _beatCount - 2)
			{
				_shakeVal = 0;
				_isShowShadow = true;
			}
			else if (_moveCount == _beatCount - 3)
			{
				_isShowShadow = false;
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);

				if (_isLeft)
				{
					makeEnemyIdx({ _player->getIdx().x + RESPAWN_LEFT.x, _player->getIdx().y + RESPAWN_LEFT.y });
				}
				else if (!_isLeft)
				{
					makeEnemyIdx({ _player->getIdx().x + RESPAWN_RIGHT.x, _player->getIdx().y + RESPAWN_RIGHT.y });
				}
				(*_vvMap)[_idx.y][_idx.x]->setWaterTile();
			}
			else if (_moveCount == _beatCount - 4 || _moveCount == _beatCount - 5)
			{
				_shakeVal = 3;
				if ((_player->getIdx().x == _idx.x - 1 && _player->getIdx().y == _idx.y) ||
					(_player->getIdx().x == _idx.x + 1 && _player->getIdx().y == _idx.y) ||
					(_player->getIdx().x == _idx.x && _player->getIdx().y == _idx.y - 1) ||
					(_player->getIdx().x == _idx.x && _player->getIdx().y == _idx.y + 1))
				{
					if (!_player->getIsHit())
					{
						(*_player).setCurHp((*_player).getCurHp() - _dmg);

						showAttackEffect();

						_player->setIsHit(true);
					}
				}
			}
			else if (_moveCount == _beatCount - 6)
			{
				_shakeVal = 0;
			}
			else if (_moveCount == _beatCount - 7)
			{
				(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
				makeEnemyIdx({ (int)(_savePos.x / TILESIZE), (int)(_savePos.y / TILESIZE) });
				_moveCount += 24;

				_player->setIsHit(false);
			}
			_posCT.x = _posLT.x + TILESIZE / 2;
			_posCT.y = _posLT.y + TILESIZE / 2;
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
			_idx.x = (_posLT.x) / TILESIZE;
			_idx.y = (_posLT.y) / TILESIZE;
			(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
		}
		else if (_isFind)
		{
			if (_beatCount == 1)
			{
				_shakeVal = 3;
			}
			else if (_beatCount == 2)
			{
				_beatCount = 0;
				_shakeVal = 0;
				if (_listPath.size() > 0)
				{
					_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
				}
				else
				{
					POINT tmp = { RND->getInt(2),RND->getInt(2) };
					_listPath.push_back(tmp);
					_direction = { _listPath.begin()->x - _idx.x , _listPath.begin()->y - _idx.y };
				}

				if ((*_player).getIdx().x == _idx.x + _direction.x && (*_player).getIdx().y == _idx.y + _direction.y)
				{
					(*_player).setCurHp((*_player).getCurHp() - _dmg);

					showAttackEffect();
					//SOUNDMANAGER->playEff("skeleton_Attack");
				}
				else if((*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->getIsAvailMove() == false)
				{}
				else if (_direction.x == 0)
				{
					if (_direction.y == 1)
					{
						_isMove = true;
						_vec.y += SPEED;
					}
					else if (_direction.y == -1)
					{
						_isMove = true;
						_vec.y -= SPEED;
					}
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				}
				else if (_direction.y == 0)
				{
					if (_direction.x == 1)
					{
						_isMove = true;
						_vec.x += SPEED;
					}
					else if (_direction.x == -1)
					{
						_isMove = true;
						_vec.x -= SPEED;
					}
					(*_vvObj)[_idx.y + _direction.y][_idx.x + _direction.x]->setIsAvailMove(false);
				}
			}
		}
	}
	if (_shakeVal != 0)
	{
		_shakeCount++;
		if (_shakeCount >= 3)
		{
			_shakeCount = 0;
			_shakeVal == 3 ? _shakeVal = -3 : _shakeVal = 3;
		}
	}
	if (_isMove)
	{
		moveCal();
	}
}

void violin::render()
{
	//D2DMANAGER->fillRectangle(0xff00ff, _posLT.x - CAMERA->getPosX(), _posLT.y - CAMERA->getPosY(), _posLT.x + TILESIZE - CAMERA->getPosX(), _posLT.y + TILESIZE - CAMERA->getPosY(), 1.0f);

	if (!_isShowShadow)
	{
		if (_isLeft)
		{
			_img->aniRender(_posLT.x - CAMERA->getPosX() + _shakeVal - MINUSX_LEFT, _posLT.y - _posZ - CAMERA->getPosY() - MINUSY, _ani);
		}
		else if (!_isLeft)
		{
			_img->aniRenderReverseX(_posLT.x - CAMERA->getPosX() + _shakeVal - MINUSX_RIGHT, _posLT.y - _posZ - CAMERA->getPosY() - MINUSY, _ani);
		}
	}
	else if (_isShowShadow)
	{
		_shadowImg->render(_posLT.x - CAMERA->getPosX() + 10, _posLT.y - CAMERA->getPosY() + 15, 1.0f);
	}
}

void violin::imageInit()
{
	IMAGEMANAGER->addFrameImage("violin", L"images/monster/boss/coralriff/coralriff_strings.png", 360, 224, 4, 2);
	_img = IMAGEMANAGER->findImage("violin");

	KEYANIMANAGER->addAnimationType("violin");

	int stand[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("violin", "violin_Stand", "violin", stand, 4, ANIMATION_FPS, true);

	int shadow[] = { 4,5,6,7 };
	KEYANIMANAGER->addArrayFrameAnimation("violin", "violin_Shadow", "violin", shadow, 4, ANIMATION_FPS, true);

	_ani = KEYANIMANAGER->findAnimation("violin", "violin_Stand");
	_ani->start();

	IMAGEMANAGER->addImage("shadow_Tell", L"images/monster/boss/coralriff/tentacle_tell.png", 30, 20);

	_shadowImg = IMAGEMANAGER->findImage("shadow_Tell");

	_dustImg = nullptr;
	_dustAni = nullptr;
}

void violin::moveCal()
{
	if (_posLT.x - _savePos.x >= TILESIZE || _savePos.x - _posLT.x >= TILESIZE)
	{
		if (_vec.x > 0)
		{
			_posLT.x = _savePos.x + TILESIZE;
		}
		else if (_vec.x < 0)
		{
			_posLT.x = _savePos.x - TILESIZE;
		}
		_vec.x = 0;
		_savePos = _posLT;
		_isMove = false;
	}

	if (_posLT.y - _savePos.y >= TILESIZE || _savePos.y - _posLT.y >= TILESIZE)
	{
		if (_vec.y > 0)
		{
			_posLT.y = _savePos.y + TILESIZE;
		}
		else if (_vec.y < 0)
		{
			_posLT.y = _savePos.y - TILESIZE;
		}
		_vec.y = 0;
		_savePos = _posLT;
		_isMove = false;

	}


	_posLT.x += _vec.x;
	_posLT.y += _vec.y;
	_posCT.x = _posLT.x + TILESIZE / 2;
	_posCT.y = _posLT.y + TILESIZE / 2;
	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(true);
	_idx.x = (_posLT.x) / TILESIZE;
	_idx.y = (_posLT.y) / TILESIZE;
	if (_isMove == false)
	{
		(*_vvObj)[_idx.y - _direction.y][_idx.x - _direction.x]->setIsAvailMove(true);
		_direction = { 0,0 };
	}
	(*_vvObj)[_idx.y][_idx.x]->setIsAvailMove(false);
}

void violin::showAttackEffect()
{
	EFFECTMANAGER->play("enemy_attack", _player->getPosCT().x - CAMERA->getPosX(), _player->getPosCT().y - CAMERA->getPosY());
}
