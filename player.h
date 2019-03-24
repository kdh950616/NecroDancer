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

class enemyMgr;

class player : public gameNode
{
private:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;


private:

	vvMap* _vvMap;					// ���������� �������ִ� ���� �ּҸ� ������ ��������(�뷮���� ��).
	vvObj* _vvObj;					//			''		������Ʈ				''
	enemyMgr* _em;

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
	vector<POINT> _vDirection;


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
	bool _isPressKey;				//Ű������?
	bool _isBeat;					//��Ʈ�� �´�?
	bool _isHit;					//�¾Ҵ�?
	

private:
	//=========================
	//			������
	//=========================

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

	void attackFunc();
	//===========================================
	//					link
	//===========================================

	void setMapAdressLink(vvMap* vvMap) { _vvMap = vvMap; }
	void setObjAdressLink(vvObj* vvObj) { _vvObj = vvObj; }
	void linkEnemyMgr(enemyMgr* em) { _em = em; }

	//===========================================
	//					init
	//===========================================
	void imgInit();

	//===========================================
	//					update
	//===========================================
	void move();
	void moveCal();
	void objFunc(POINT direction);
	void enemyAtk();

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
	void		setHeadImg			(image* headImg)			{ _headImg = headImg; }
	void		setBodyImg			(image* bodyImg)			{ _bodyImg = bodyImg; }
	void		setHeadAni			(animation* headAni)		{ _headAni = headAni; }
	void		setBodyAni			(animation* bodyAni)		{ _bodyAni = bodyAni; }
	void		setPosCT			(POINTFLOAT posCT)			{ _posCT = posCT; }
	void		 setPosLT			(POINTFLOAT posLT)			{ _posLT = posLT; }
	void		setSavePos			(POINTFLOAT savePos)		{ _savePos = savePos; }
	void		setVec				(POINTFLOAT vec)			{ _vec = vec; }
	void		setIdx				(POINT idx)					{ _idx = idx; }
	void		setRc				(D2D_RECT_F rc)				{ _rc = rc; }
	void		setMaxHp			(int maxHp)					{ _maxHp = maxHp; }
	void		setCurHp			(int curHp)					{ _curHp = curHp; }
	void		setJumpPower		(float jumpPower)			{_jumpPower = jumpPower; }

	//======================
	//	�Ｎ �߰� ������
	//======================
	bool getIsMove() { return _isMove; }
	float getPosZ() { return _posZ; }
	bool getIsReverse() { return _isReverse; }
	bool getIsArrive() { return _isArrive; }
	void setIsArrive(bool isArrive) { _isArrive = isArrive; }
	bool getIsBeat() { return _isBeat; }
	void setIsBeat(bool isBeat) { _isBeat = isBeat; }
	bool getIsPressKey() { return _isPressKey; }
	void setIsPressKey(bool isPressKey) { _isPressKey = isPressKey; }
	POINT getDirection() { return _direction; }
	bool getIsHit() { return _isHit; }
	void setIsHit(bool isHit) { _isHit = isHit; }
};

