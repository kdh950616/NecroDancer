#pragma once
#include "gameNode.h"
#include "tile.h"
#include "lightMap.h"

//enum tagEnemy 
//{
//	ENEMY1_1_1, // 검은박쥐
//	ENEMY1_1_2,	// 빨간박쥐
//	ENEMY1_2_1,	// 하얀해골
//	ENEMY1_2_2,	// 까만해골
//	ENEMY1_2_3,	// 노란해골
//	ENEMY1_3_1,	// 초록슬라임
//	ENEMY1_3_2,	// 파란슬라임
//	ENEMY1_3_3,	// 노란슬라임
//	ENEMY2_1_1,	// 유령
//	ENEMY2_1_2,	// 레이스
//	ENEMY2_2_1,	// 원숭이
//	ENEMY2_2_2,	// 좀비
//	ENEMY3_1_1,	// 미노타우로스
//	ENEMY3_1_2,	// 용
//	ENEMY3_1_3,	// 황금박쥐
//	ENEMY4_1_1,	// 폰
//	ENEMY4_1_2,	// 비숍
//	ENEMY4_1_3,	// 룩
//	ENEMY4_2_1,	// 나이트
//	ENEMY4_2_2,	// 퀸
//	ENEMY4_2_3,	// 킹
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
	CORALRIFF,	//윤쟁누나
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
	//					주소 받을 변수들
	//=====================================================
	player* _player; //플레이어 주소 받을곳
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
	//해골같은경우는 체력이 2이상일때 체력 1이되면 머리떨어지고 도망가야하니까 해골한테 따로 상태불값 주자.

	float _jumpPower;
	float _gravity;
	float _posZ;
	float _speed;


	bool _isBeat;		// 비트니?
	bool _isMove;		// 움직여야하니? -> 트루면 움직임값 한번주고 바로 펄스
	bool _isLeft;		// 렌더용 -> 트루면 왼쪽방향 이미지 출력 펄스면 오른쪽방향
	bool _isFind;		// 발견했니? 발견했을떄만 쫓아오는놈들을 위해 만듬
	bool _isLight;		// 밝은상태니? -> 펄스면 그림자 이미지로
	bool _isSpecialAct; // 특수상태니? (해골 머리떨어지기, 원숭이 붙잡기상태 등)  or 코랄리프는 이거 트루되면 에이스타로 쫓아옴
	bool _isBackward;	// 도망가다가 벽에 박으면 절반갔다가 되돌아옴
	bool _isNeedAstar;	// 에이스타 필요하니?


	//그림자
	image* _shadowImg;

	//흙먼지
	image* _dustImg;
	animation* _dustAni;

	//가야할곳 구하기용
	list<POINT> _listPath;
	list<POINT>::iterator _listIPath;
	
	//코랄리프용
	int _moveCount;
	bool _isShowShadow;
	int _shakeVal;
	int _shakeCount = 0;
	bool _isAttack;

	//딥블루스용

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
	//					즉석 추가 목록
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

