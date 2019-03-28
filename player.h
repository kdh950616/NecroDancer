#pragma once
#include "gameNode.h"
#include "tile.h"

enum tagItemKind
{
	INVEN_WEAPON,
	INVEN_TORCH,
	INVEN_SHOVEL,
	INVEN_ARMOR,
	INVEN_HP,
	INVEN_END
};

enum tagWeaponType
{
	WEAPON_DAGGER,
	WEAPON_SPEAR,
	WEAPON_SWORD
};

struct tagInventory
{
	image* img;		//아이템 이미지 = 맵툴에있는 item이미지쓸거
	image* slotImg;
	int frameX;
	int frameY;
	int itemKind;	//아이템 종류  ex) 무기, 횃불, 삽 등
	bool isGlass;	//유리아이템인지?
	bool isGet;		//아이템획득했니? 
	int itemVal;	//아이템값 ex) ITEM_TORCH_GLASS, ITEM_SPEAR_BLOOD 등 
};

class enemyMgr;

class player : public gameNode
{
private:
	typedef vector<tile*> vLine;
	typedef vector<vLine> vvMap;
	typedef vector<vLine> vvObj;


private:

	vvMap* _vvMap;					// 스테이지가 가지고있는 맵의 주소를 가지고 있을거임(용량걱정 ㄴ).
	vvObj* _vvObj;					//			''		오브젝트				''
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

	POINTFLOAT _posLT;				//플레이어 레프트 탑 좌표(모든 좌표는 이 좌표로부터 - or +
	POINTFLOAT _posCT;				//플레이어 센터 좌표 
	POINTFLOAT _savePos;
	POINTFLOAT _vec;				//플레이어 좌표 이동해야할 수치.

	//=========================
	//			POINT
	//=========================

	POINT _idx;						//플레이어가 서있는 타일의 인덱스
	POINT _direction;				//플레이어가 누를값에따라 바뀜
	vector<POINT> _vDirection;		//무기에따라 바뀜


	//=========================
	//			RECT
	//=========================

	D2D_RECT_F _rc;					//플레이어 렉트

	//=========================
	//			int
	//=========================

	int _maxHp;						//최대체력
	int _curHp;						//현재체력

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
	bool _isPressKey;				//키눌럿니?
	bool _isBeat;					//비트에 맞니?
	bool _isHit;					//맞았니?
	

private:
	//=========================
	//			아이템
	//=========================

	tagInventory _inventory[INVEN_END];
	int _bloodKill;					//피무기일때 킬횟수 10되면 피1회복하고 0으로다시만듬
	int _dmg;						//데미지
	int _def;						//방어력
	int _torchPower;				//횃불세기
	int _recoverHp;				//체력회복수치

	//삽
	image* _shovelImg;			//삽이미지
	int _shovelFrameX;
	int _shovelFrameY;
	int _shovelDmg;				//삽데미지
	bool _isShowShovel;			//삽보여주니?
	int _shovelCount;		//삽보여주는시간

	//공격관련
	image* _effectImg;
	float _effectAngle;
	bool _isShowEff;
	int _effCount;
	int _atkCombo;

	//코인배수 관련
	int _grooveChain;
	int _killCombo;

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
	void linkEnemyMgr(enemyMgr* em) { _em = em; }

	//===========================================
	//					init
	//===========================================
	void imgInit();
	void inventoryInit();

	//===========================================
	//					update
	//===========================================
	void move();
	void moveCal();
	void objFunc(POINT direction);
	void setInventoryItem(int objAttr);
	void attackFunc(POINT direction);
	void setWeaponVDirection(int itemVal, POINT direction);
	void useHpIten();
	//===========================================
	//					render
	//===========================================

	void renderUI();
	void txtRender();
	void effectRender();

	//===========================================
	//					즉석추가 기능함수
	//===========================================

	void brokeWeapon();
	void brokeTorch();
	void brokeShovel();
	void brokeArmor();

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
	//	즉석 추가 접근자
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
	int getDef() { return _def; }
	void setDef(int def) { _def = def; }

	int getTorchPower() { return _torchPower; }
	void setTorchPowet(int torchPower) { _torchPower = torchPower; }
	tagInventory getInventory(int arrayNum) { return _inventory[arrayNum]; }
	void setBloodKill(int bloodKill) { _bloodKill = bloodKill; }
	int getBloodKill() { return _bloodKill; }
	void addBloodKill(int addNum) { _bloodKill += addNum; }

	void setGrooveChain(int grooveChain) { _grooveChain = grooveChain; }
	int getGrooveChain() { return _grooveChain; }
	void setKillCombo(int killCombo) { _killCombo = killCombo; }
	int getKillCombo() { return _killCombo; }
};

