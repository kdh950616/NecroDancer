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

	vvMap* _vvMap;					// ���������� �������ִ� ���� �ּҸ� ������ ��������(�뷮���� ��).
	vvObj* _vvObj;					//			''		������Ʈ				''

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

	POINTFLOAT _posLT;				//�÷��̾� ����Ʈ ž ��ǥ(��� ��ǥ�� �� ��ǥ�κ��� - or +
	POINTFLOAT _posCT;				//�÷��̾� ���� ��ǥ 
	POINTFLOAT _savePos;
	POINTFLOAT _vec;				//�÷��̾� ��ǥ �̵��ؾ��� ��ġ.

	//=========================
	//			POINT
	//=========================

	POINT _idx;						//�÷��̾ ���ִ� Ÿ���� �ε���
	POINT _direction;

	//=========================
	//			RECT
	//=========================

	D2D_RECT_F _rc;					//�÷��̾� ��Ʈ

	//=========================
	//			int
	//=========================

	int _maxHp;						//�ִ�ü��
	int _curHp;						//����ü��
	int _dmg;						//������

	tagShovel _shovel;
	int _shovel_Dmg;				//�𵥹���

	//=========================		
	//			float 				
	//=========================		

	float _speed;					//�ӵ�
	float _jumpPower;				//�����Ŀ�(�̹��� ��� ����)
	float _posZ;					//�̹��� ��¿� z��ǥ

	//=========================		
	//			bool 				
	//=========================	

	bool _isMove;					//�����̴� ���̴�?
	bool _isReverse;				//�ݴ��?
	bool _isArrive;					//�����ߴ�? (�̵��ϴٰ�)
	

private:
	//=========================
	//			������
	//=========================
	const float GRAVITY = 1.2f;
	const float JUMPPOWER_HORIZON = 8.0f;
	const float JUMPPOWER_VERTICAL = 6.0f;
	const float SPEED = 5.0f;
	//=========================
	//			��ǥ��
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
	//	�Ｎ �߰� ������
	//======================
	bool getIsMove() { return _isMove; }
	float getPosZ() { return _posZ; }
	bool getIsReverse() { return _isReverse; }
	bool getIsArrive() { return _isArrive; }
	void setIsArrive(bool isArrive) { _isArrive = isArrive; }
};

