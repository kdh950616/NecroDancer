#pragma once
#include "gameNode.h"
#include "tile.h"
#include "lightMap.h"

//enum tagEnemy 
//{
//	ENEMY1_1_1, // ��������
//	ENEMY1_1_2,	// ��������
//	ENEMY1_2_1,	// �Ͼ��ذ�
//	ENEMY1_2_2,	// ��ذ�
//	ENEMY1_2_3,	// ����ذ�
//	ENEMY1_3_1,	// �ʷϽ�����
//	ENEMY1_3_2,	// �Ķ�������
//	ENEMY1_3_3,	// ���������
//	ENEMY2_1_1,	// ����
//	ENEMY2_1_2,	// ���̽�
//	ENEMY2_2_1,	// ������
//	ENEMY2_2_2,	// ����
//	ENEMY3_1_1,	// �̳�Ÿ��ν�
//	ENEMY3_1_2,	// ��
//	ENEMY3_1_3,	// Ȳ�ݹ���
//	ENEMY4_1_1,	// ��
//	ENEMY4_1_2,	// ���
//	ENEMY4_1_3,	// ��
//	ENEMY4_2_1,	// ����Ʈ
//	ENEMY4_2_2,	// ��
//	ENEMY4_2_3,	// ŷ
//};

enum tagEnemyType
{
	BAT,
	SKELETON,
	SLIME,
	ZOMBIE,
	WRAITH,
	CLONE,
	ARMADILLO,
	MINOTAUR,
	DRAGON,
	BAT_BOSS,
	BANSHEE,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	CORALRIFF,	//���ﴩ��
	KEYBORD,
	VIOLIN,
	HORN,
	DRUM,
	END
};

class player;

class enemy : public gameNode
{
protected:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;
	typedef vector<lightMap*> vLightLine;
	typedef vector<vLightLine> vvLightMap;
	//=====================================================
	//					�ּ� ���� ������
	//=====================================================
	player* _player; //�÷��̾� �ּ� ������
	vvMap* _vvMap;
	vvObj* _vvObj;
	vvLightMap* _vvLightMap;
protected:
	image* _img;
	animation* _ani;

	POINTFLOAT _posLT;
	POINTFLOAT _posCT;
	POINTFLOAT _savePos;
	POINTFLOAT _vec;
	POINT		_idx;
	POINT		_direction;

	D2D1_RECT_F _rc;

	int _enemyType;
	int _maxHp;
	int _curHp;

	int _dmg;
	int _beatCount;
	//�ذ������� ü���� 2�̻��϶� ü�� 1�̵Ǹ� �Ӹ��������� ���������ϴϱ� �ذ����� ���� ���ºҰ� ����.

	float _jumpPower;
	float _gravity;
	float _posZ;
	float _speed;


	bool _isBeat;		// ��Ʈ��?
	bool _isMove;		// ���������ϴ�? -> Ʈ��� �����Ӱ� �ѹ��ְ� �ٷ� �޽�
	bool _isLeft;		// ������ -> Ʈ��� ���ʹ��� �̹��� ��� �޽��� �����ʹ���
	bool _isFind;		// �߰��ߴ�? �߰��������� �Ѿƿ��³���� ���� ����
	bool _isLight;		// �������´�? -> �޽��� �׸��� �̹�����
	bool _isSpecialAct; // Ư�����´�? (�ذ� �Ӹ���������, ������ �������� ��)  or �ڶ������� �̰� Ʈ��Ǹ� ���̽�Ÿ�� �Ѿƿ�
	bool _isBackward;	// �������ٰ� ���� ������ ���ݰ��ٰ� �ǵ��ƿ�
	bool _isNeedAstar;	// ���̽�Ÿ �ʿ��ϴ�?


	//�׸���
	image* _shadowImg;

	//�����
	image* _dustImg;
	animation* _dustAni;

	//�����Ұ� ���ϱ��
	list<POINT> _listPath;
	list<POINT>::iterator _listIPath;
	
	//�ڶ�������
	int _moveCount;
	bool _isShowShadow;
	int _shakeVal;
	int _shakeCount = 0;
	bool _isAttack;

	//����罺��

public: 
	enemy();
	~enemy();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	//=============================================

	virtual void makeEnemyIdx(POINT idx);
	virtual void unfoundFunc();
	virtual void lightFunc();
	virtual void showAttackEffect();
	//=============================================
	//					link
	//=============================================

	void linkPlayer(player* player) { _player = player; }
	void linkMap(vvMap* vvMap) { _vvMap = vvMap; }
	void linkObj(vvObj* vvObj) { _vvObj = vvObj; }
	void linkLightMap(vvLightMap* vvLightMap) { _vvLightMap = vvLightMap; }

	//=============================================
	//					get
	//=============================================
	image*			getImg()		{ return _img; }
	animation*		getAni()		{ return _ani; }

	POINTFLOAT		getPosLT()		{ return _posLT; }
	POINTFLOAT		getPosCT()		{ return _posCT; }
	POINT			getIdx()		{ return _idx; }
	
	D2D1_RECT_F		getRc()			{ return _rc; }
 
	int				getMaxHp()		{ return _maxHp; }
	int				getCurHp()		{ return _curHp; }

	int				getDmg()		{ return _dmg; }

	float			getJumpPower()	{ return _jumpPower; }
	float			getGravity()	{ return _gravity; }
	float			getPosZ()		{ return _posZ; }

	bool			 getIsMove()	{ return _isMove; }

	//=============================================
	//					set
	//=============================================
	void setImg(image* img)						{ _img = img; }
	void setAni(animation* ani)					{ _ani = ani; }

	void setPosLT(POINTFLOAT posLT)				{ _posLT = posLT; }
	void setPosCT(POINTFLOAT posCT)				{ _posCT = posCT; }
	void	setIdx(POINT idx)					{ _idx = idx; }

	void setRc(D2D1_RECT_F rc)					{ _rc = rc; }

	void setMaxHp(int maxHp)					{ _maxHp = maxHp; }
	void setCurHp(int curHp)					{ _curHp = curHp; }
	
	void setDmg(int dmg)						{ _dmg = dmg; }
	
	void setJumpPower(float jumpPower)			{ _jumpPower = jumpPower; }
	void setGravity(float gravity)				{ _gravity = gravity; }
	void setPosZ(float posZ)					{ _posZ = posZ; }
	
	void setIsMove(bool isMove)					{ _isMove = isMove; }
	//=============================================
	//					�Ｎ �߰� ���
	//=============================================
	void setIsBeat(bool isBeat)					{ _isBeat = isBeat; }
	list<POINT>* getListPath()					{ return &_listPath; }
	bool getIsSpecialAct()						{ return _isSpecialAct; }
	int getEnemyType()							{ return _enemyType; }
	bool getIsNeedAstar()						{ return _isNeedAstar; }
	int getBeatCount()							{ return _beatCount; }
	void setBeatCount(int beatCount)			{ _beatCount = beatCount; }
	void setIsFind(bool isFind) { _isFind = isFind; }
	bool getIsFind() { return _isFind; }
	void setDirection(POINT direction) { _direction = direction; }
	POINT getDirection() { return _direction; }
	virtual bool getIsStun() { return 0; }
};

