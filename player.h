#pragma once
#include "gameNode.h"
#include "tile.h"

enum tagShovel
{
	BASIC,
	SHARD,
	OBSIDIAN,
	BLOOD,
	CRYSTAL,
	GLASS,
	TITANIUM
};

class player : public gameNode
{
private:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;


private:

	vvMap* _vvMap;					// 스테이지가 가지고있는 맵의 주소를 가지고 있을거임(용량걱정 ㄴ).
	vvObj* _vvObj;					//			''		오브젝트				''

	//=========================
	//			image
	//=========================

	image* _headImg;
	image* _bodyImg;
	image* _shadowImg;

	//=========================
	//		animation
	//=========================

	animation* _headAni;
	animation* _bodyAni;

	//=========================
	//		effect
	//=========================

	//=========================
	//		POINTFLOAT
	//=========================

	POINTFLOAT _posLT;				//플레이어 레프트 탑 좌표(모든 좌표는 이 좌표로부터 - or +
	POINTFLOAT _posCT;				//플레이어 센터 좌표 
	POINTFLOAT _savePos;
	POINTFLOAT _vec;				//플레이어 좌표 이동해야할 수치.

	//=========================
	//			POINT
	//=========================

	POINT _idx;						//플레이어가 서있는 타일의 인덱스
	POINT _direction;

	//=========================
	//			RECT
	//=========================

	D2D_RECT_F _rc;					//플레이어 렉트

	//=========================
	//			int
	//=========================

	int _maxHp;						//최대체력
	int _curHp;						//현재체력
	int _dmg;						//데미지

	tagShovel _shovel;
	int _shovel_Dmg;				//삽데미지

	//=========================		
	//			float 				
	//=========================		

	float _speed;					//속도
	float _jumpPower;				//점프파워(이미지 출력떄 쓰임)
	float _posZ;					//이미지 출력용 z좌표

	//=========================		
	//			bool 				
	//=========================	

	bool _isMove;					//움직이는 중이니?
	bool _isReverse;				//반대니?
	bool _isArrive;					//도착했니? (이동하다가)
	

private:
	//=========================
	//			변수용
	//=========================
	const float GRAVITY = 1.2f;
	const float JUMPPOWER_HORIZON = 8.0f;
	const float JUMPPOWER_VERTICAL = 6.0f;
	const float SPEED = 5.0f;
	//=========================
	//			좌표용
	//=========================

public:
	player();
	~player();

	HRESULT init();
	void release();
	void update();
	void render();

	//===========================================
	//					link
	//===========================================
	void setMapAdressLink(vvMap* vvMap) { _vvMap = vvMap; }
	void setObjAdressLink(vvObj* vvObj) { _vvObj = vvObj; }
	//===========================================
	//					init
	//===========================================
	void imgInit();

	//===========================================
	//					update
	//===========================================
	void move();
	void objFunc(POINT direction);

	//===========================================
	//					render
	//===========================================


	//===========================================
	//					get
	//===========================================
	image*		getHeadImg()		{ return _headImg; }
	image*		getBodyImg()		{ return _bodyImg; }
	animation*	getHeadAni()		{ return _headAni; }
	animation*	getBodyAni()		{ return _bodyAni; }
	POINTFLOAT	getPosCT()			{ return _posCT; }		
	POINTFLOAT	getPosLT()			{ return _posLT; }
	POINTFLOAT	getSavePos()		{ return _savePos; }
	POINTFLOAT	getVec()			{ return _vec; }				
	POINT		getIdx()			{ return _idx; }				
	D2D_RECT_F	getRc()				{ return _rc; }				
	int			getMaxHp()			{ return _maxHp; }			
	int			getCurHp()			{ return _curHp; }			
	float		getJumpPower()		{ return _jumpPower; }		

	//===========================================
	//					set
	//===========================================
	image*		setHeadImg			(image* headImg)			{ _headImg = headImg; }
	image*		setBodyImg			(image* bodyImg)			{ _bodyImg = bodyImg; }
	animation*	setHeadAni			(animation* headAni)		{ _headAni = headAni; }
	animation*	setBodyAni			(animation* bodyAni)		{ _bodyAni = bodyAni; }
	POINTFLOAT	setPosCT			(POINTFLOAT posCT)			{ _posCT = posCT; }
	POINTFLOAT	 setPosLT			(POINTFLOAT posLT)			{ _posLT = posLT; }
	POINTFLOAT	setSavePos			(POINTFLOAT savePos)		{ _savePos = savePos; }
	POINTFLOAT	setVec				(POINTFLOAT vec)			{ _vec = vec; }
	POINT		setIdx				(POINT idx)					{ _idx = idx; }
	D2D_RECT_F	setRc				(D2D_RECT_F rc)				{ _rc = rc; }
	int			setMaxHp			(int maxHp)					{ _maxHp = maxHp; }
	int			setCurHp			(int curHp)					{ _curHp = curHp; }
	float		setJumpPower		(float jumpPower)			{_jumpPower = jumpPower; }

	//======================
	//	즉석 추가 접근자
	//======================
	bool getIsMove() { return _isMove; }
	float getPosZ() { return _posZ; }
	bool getIsReverse() { return _isReverse; }
	bool getIsArrive() { return _isArrive; }
	void setIsArrive(bool isArrive) { _isArrive = isArrive; }
};

