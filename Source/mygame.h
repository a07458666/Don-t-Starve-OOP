/*
 * mygame.h: ���ɮ��x�C��������class��interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID  				// �w�q�U�ح��Ī��s��
{
	AUDIO_relax,			// 0
	AUDIO_keyin,			// 1
	AUDIO_keydown,			// 2
	AUDIO_walk,				// 3
	AUDIO_NightBGM,			// 4
	AUDIO_NightToSunBGM,	// 5
	AUDIO_SunBGM,			// 6
	AUDIO_SunToNightBGM,    // 7
	AUDIO_pick,             // 8
	AUDIO_Get,              // 9
	AUDIO_tap,              //10
	AUDIO_felling,          //11
	AUDIO_OVER,				//12
	AUDIO_doghit,			//13
	AUDIO_Hited,            //14
	AUDIO_Eat,				//15
	AUDIO_Slide				//16

};

namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// �a�ϯ���
	/////////////////////////////////////////////////////////////////////////////
	class CRole;
	class CgameMap;
	class CItem;
	class CMapstuff
	{
	public:
		CMapstuff(int BlockX=0,int BlockY=0);
		bool HitRole(CRole* Role);								// �O�_�I�쨤��
		bool IsAlive();											// �O�_����
		virtual void LoadBitmap()=0;							// ���J�ϧ�
		void OnMove(CgameMap* Map);								// ���ʨ���(��ڹC�����S���Ψ�)
		virtual void OnShow()=0;								// �N�ϧζK��e��
		void SetBlockXY(int nx, int ny);						// �]�w��檺�y��
		void SetIsAlive(bool alive);							// �]�w�O�_����
		int GetX1();
		int GetX2();
		int GetY1();
		int GetY2();
	protected:
		CMovingBitmap stuffImg;			//�Ϥ�
		int x, y;						//���⥪�W�����y��
		int life;						//�ͩR
		bool is_alive;					// �O�_����
		int BlockX, BlockY;				//���y����a�ϥ�
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
	/////////////////////////////////////////////////////////////////////////////
	// ���flint
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_flint: public CMapstuff
	{
	public:
		CMapstuff_flint(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void OnShow();											// �N�ϧζK��e��
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
	/////////////////////////////////////////////////////////////////////////////
	// ��
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_tree : public CMapstuff
	{
	public:
		CMapstuff_tree(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void OnShow();											// �N�ϧζK��e��
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
	/////////////////////////////////////////////////////////////////////////////
	// ���Y
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Axes : public CMapstuff
	{
	public:
		CMapstuff_Axes(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void OnShow();											// �N�ϧζK��e��
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
	/////////////////////////////////////////////////////////////////////////////
	// ����
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Hay : public CMapstuff
	{
	public:
		CMapstuff_Hay(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void OnShow();											// �N�ϧζK��e��
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
	/////////////////////////////////////////////////////////////////////////////
	// ��KBranch
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Branch : public CMapstuff
	{
	public:
		CMapstuff_Branch(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void OnShow();											// �N�ϧζK��e��
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
	/////////////////////////////////////////////////////////////////////////////
	// ���Ystone(6)
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_stone : public CMapstuff
	{
	public:
		CMapstuff_stone(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void OnShow();											// �N�ϧζK��e��
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
	/////////////////////////////////////////////////////////////////////////////
	// �U�lHat(12)
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Hat : public CMapstuff
	{
	public:
		CMapstuff_Hat(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void OnShow();											// �N�ϧζK��e��
	private:
		int life;
	};
	/////////////////////////////////////////////////////////////////////////////
	// ����gold(13)
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_gold : public CMapstuff
	{
	public:
		CMapstuff_gold(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void OnShow();											// �N�ϧζK��e��
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
	/////////////////////////////////////////////////////////////////////////////
	// ��ù�iCarrot(14)
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Carrot : public CMapstuff
	{
	public:
		CMapstuff_Carrot(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void OnShow();											// �N�ϧζK��e��
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
	/////////////////////////////////////////////////////////////////////////////
	// �ߪGBerry(15)
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Berry : public CMapstuff
	{
	public:
		CMapstuff_Berry(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void OnShow();											// �N�ϧζK��e��
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
	/////////////////////////////////////////////////////////////////////////////
	// �ߤlRabbit
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Rabbit : public CMapstuff
	{
	public:
		CMapstuff_Rabbit(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void Initialize();
		void OnShow();											// �N�ϧζK��e��
		void OnMove(CgameMap *Map);
		void avoidMove(CgameMap *Map, CRole *Role);
	protected:
		CAnimation animationR, animationL;
		CMovingBitmap Rstop, Lstop;
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
		int MoveTimes, dir, olddir;
		int nX, nY;
		int life;
	};
	/////////////////////////////////////////////////////////////////////////////
	// �ª�blockdog
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_BlockDog :public CMapstuff
	{
	public:
		CMapstuff_BlockDog(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void Initialize();										//��l��
		void OnShow();											// �N�ϧζK��e��
		void OnMove(CgameMap *Map);								//�@�벾��
		void TraceMove(CgameMap *Map, CRole *Role);				//�l�H�Ҧ�
		void Setfight(bool is);									//�P�_�����Z��
		bool GetHit();											//�O�_����
		int  Getlife();											//���o�ª��ͩR��
		int  OnHit();											//�������
	protected:
		CAnimation animationR, animationL;						//���k���ʰʵe
		CAnimation Rstop, Lstop,fightR,fightL;					//���k���y�B�����ʵe
	private:
		int MoveTimes, MoveTimesHit, dir, olddir;				//���ʮɶ��B�����ɶ��B��V�B�e�@����V
		int nX, nY;												//�y��
		int life;												//�����ͩR
		bool Is_fight;											//�O�_���������A
		bool Hit;												//�O�_�����˪��A
	};
	/////////////////////////////////////////////////////////////////////////////
	// ���campfire
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_campfire : public CMapstuff
	{
	public:
		CMapstuff_campfire(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// ���J�ϧ�
		void Initialize();
		void OnShow();											// �N�ϧζK��e��
		void SetCampfire(int x, int y, CgameMap *Map);			//�N����]�w�ܦa�ϤW
		void OnMove(CgameMap *Map);
		void SetTime(bool flag);								//�]�w�դѱߤW����Ϥ�
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
		int inX, inY;											//�Q�гy����m
		bool time;
		CMovingBitmap BCampFire;								//�ߤW���ä���
	};
	/////////////////////////////////////////////////////////////////////////////
	// �a�����O
	/////////////////////////////////////////////////////////////////////////////
	class CgameMap{
	public:
		CgameMap();
		void  Initialize();				// �]�w�a�Ϭ���l��
		void LoadBitmap();
		void OnShow();
		void OnMove(CRole *role, CMapstuff* tree, CMapstuff* stone);
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		int GetX1();
		int GetX2();
		int GetY1();
		int GetY2();
		void SetHit(bool hit);			//�]�w�I��
		bool GetHit();					//���o�I��
		void SetXY(int nx, int ny);		// �]�w���l���W���y��
		void SetRun(bool run);			//�]�w�O�_�i�H����
		bool GetRun();					//���o�O�_�i�H����
		int GetMH();					//���o�a�Ϥ�氪��
		int GetMW();					//���o�a�Ϥ�氪��
		int Loadstuff(int x, int y);				//���J����
		int Onshowstuff();				//��ܪ���
		int GetMap_G(int x, int y);
		~CgameMap();

	private:
		CMovingBitmap ground,stone,witer,biggrass;	//�U���a��
		CMovingBitmap forest11, forest12, forest13, forest14, forest15, forest16, forest17, forest18, forest19;
		int map[20][20],map_stuff[40][40];		//�a�ϻP�D��}�C
		int x, y;
		const int MW, MH;			//�����e
		CAnimation animationMap;	// ���ʪ��ʵe
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
		bool run;					//�O�_����
		bool Hit;					//�o�͸I��
	};
/////////////////////////////////////////////////////////////////////////////
// ��
/////////////////////////////////////////////////////////////////////////////
	class CSea{
	public:
		CSea();
		void  Initialize();				// �]�w��l��
		void LoadBitmap();
		void OnShow();
		void OnMove(CgameMap *Map);
		void SetXY(int nx, int ny);		// �]�w���W���y��
		void SetRun(bool run);			//�]�w�O�_�i�H����
		bool GetRun();					//���o�O�_�i�H����
	private:
		CMovingBitmap SeaImage;
		int x, y;
		bool run;					//�O�_����
		int UpDown;					//�Ʋ���V
		int loop;					
	};
/////////////////////////////////////////////////////////////////////////////
// �ާ@�����B���k�����B�K�޻���
/////////////////////////////////////////////////////////////////////////////
class CHelp
{
	public:
		CHelp();
		void OnMove();
		void OnShow();
		void LoadBitmap();
		void SetRun(int flag);
		int  GetRun();
		int  IsMouse(CPoint *point);	//�^�ǽֳQ�I
	private:
		int x, y;
		int run,oldrun;					//���ʧ@������s��
		int movenum1,movenum2, movenum3;//���󪺦U���ʭ�
		CMovingBitmap ControlImg, GoalImg,ItemImg;//�ާ@�B�C�������D��
};
/////////////////////////////////////////////////////////////////////////////
// ��r���
/////////////////////////////////////////////////////////////////////////////
class CText
{
	public:
		CText();
		void OnMove();
		void OnShow();
		void LoadBitmap();
		void SetIsShow(bool IsShow);					//�O�_���
		bool GetIsShow();								//���o�O�_���
		void SetStoryVariable(int StoryVariable);		//�]�w�@�����`
		int  GetStoryVariable();						//���o�@�����`
		void SetText(string Text, int Txtscale);		//���J��r
		void Readfile(string filename, int RowNum, int Txtscale);//Ū�r��
		void SetKillAnimal(int ID, int num);			//����������
		int  GetKillAniml(int ID);						//���o������
	private:
		int x, y;
		bool IsShow;
		CMovingBitmap TextBackGroungImg;
		CAnimation mouse;
		string Massage;
		int movenum;
		int Txtscale;
		int StoryVariable;
		int KillRabbit, KillBilockDog;
};
/////////////////////////////////////////////////////////////////////////////
// �s�y�C
/////////////////////////////////////////////////////////////////////////////
class CMake
{
public:
	CMake();
	int  GetX1();					// �s�y���W�� x �y��
	int  GetY1();					// �s�y���W�� y �y��
	int  GetX2();					// �s�y�k�U�� x �y��
	int  GetY2();					// �s�y�k�U�� y �y��
	void Initialize();							// �]�w�s�y����l��
	void LoadBitmap();							// ���J�ϧ�
	void OnShow();								// �N�s�y�C�ϧζK��e��
	void SetXY(int nx, int ny);					// �]�w���l���W���y��
	int OnMouseDown();							//�ƹ��I��
	void SetMake_AxesOnSow(int flag);			//�]�w��ܩ��Y����
	int  GetMake_AxesOnSow();					//���o��ܩ��Y����
	void SetMake_torchfireOnSow(int flag);		//�]�w��ܤ��⤶��
	int  GetMake_torchfireOnSow();				//���o��ܤ��⤶��
	void SetMake_campfireOnSow(int flag);		//�]�w����������
	int  GetMake_campfireOnSow();				//���o����������
	void SetMake_spadeShow(int flag);			//�]�w�����l����
	int  GetMake_spadeShow();					//���o�����l����
	void SetMake_pickaxeShow(int flag);			//�]�w��ܾS�Y����
	int  GetMake_pickaxeShow();					//���o��ܾS�Y����
	void SetMake_hud_tools_Show(int flag);		//�]�w������O����
	int  GetMake_hud_tools_Show();				//���o������O����
	bool BuildHit(int x, int y);				//�سyĲ�o�ƥ�
	bool MakeformHit(int x, int y);				//�i�}Ĳ�o�ƥ�
	bool Enough(CItem item,int ID1,int amount1,int ID2,int amount2);//�O�_�i�H�سy
protected:
	CMovingBitmap Makeform, MakeBlack[10],
				  AxesYesform, AxesNoform,
				  torchfireYesform, torchfireNoform,
				  campfireYesform, campfireNoform,
				  spadeYesform,spadeNoform,
				  pickaxeYesform,pickaxeNoform,
				  hud_tools_fire, hud_tools;		// �s�@�C���ʵe
	int x, y;									// �s�@�C���W���y��
	int AxesShow, campfireShow, torchfireShow, hud_tools_Show, spadeShow,pickaxeShow;
};
/////////////////////////////////////////////////////////////////////////////
// C�D��
/////////////////////////////////////////////////////////////////////////////
class CItem_Data
{
public:
	CItem_Data();
	void Initialize();				// �]�w�s�y����l��
	void SetId(int Id);				//�]�w�D���丹
	int  GetId();					//���o�D��s��
	void Setamount(int amount);		//�]�w�ƶq
	int Getamount();				//���o�ƶq
protected:
	int x, y;					// �s�@�C���W���y��
	int ItemId;					//�D��s��
	int amount;					//�D��ƶq
private:	
};

/////////////////////////////////////////////////////////////////////////////
// �D����
/////////////////////////////////////////////////////////////////////////////

class CItem
{
public:
	CItem();
	int  GetX1();					// �s�y���W�� x �y��
	int  GetY1();					// �s�y���W�� y �y��
	int  GetX2();					// �s�y�k�U�� x �y��
	int  GetY2();					// �s�y�k�U�� y �y��
	void Initialize();				// �]�w�s�y����l��
	void LoadBitmap();				// ���J�ϧ�
	void OnShow();					// �N�s�y�C�ϧζK��e��
	void SetXY(int nx, int ny);		// �]�w���l���W���y��
	bool GetItem(int ItemId,int amount);//��o�D��
	bool LostItem(int ItemId,int amount);//���h
	bool ChaseMouse(int mX, int mY);//���_�D��
	void SetChoiceItem(int ID);//�]�w���ФW���D��
	int  GetChoiceItem();//���o���ФW���D��
	void SetfireYes(int flog);//�]�w��ܦb����Ǫ���r
	CItem_Data GetHaveItem(int ID);//���o�D������
protected:
	CMovingBitmap Itemform, Item_flint, Item_tree,// �D��C���ʵe
		Item_Axes, Item_Hay, Item_Branch,
		Item_stone, Item_Rabbit, Item_torchfire, Item_RabbitDead,
		Item_spade, Item_pickaxe,
		Item_Hat, Item_gold,
		Item_Carrot, Item_Berry,
		Item_Honey;
	CMovingBitmap face_roast, face_drop;
	int x, y;											// �s�@�C���W���y��
	CItem_Data Have_Item[19];
private:
	CInteger ItemAmount;
	int ChoiceItem, fireYes;
};

/////////////////////////////////////////////////////////////////////////////
// ���⪬�A��
/////////////////////////////////////////////////////////////////////////////
class CRoleState
{
public:
	CRoleState();
	void Initialize();				// �]�w��l��
	void LoadBitmap();				// ���J�ϧ�
	void OnShow();					// �N���A��ϧζK��e��
	void OnMove(int time, CItem Item,bool fire);//���A�ܤ�
	void SetLife(int nlife);		// �]�w�ͩR
	int GetLife();					//���o�ͩR
	void SetHungyDegree(int nlive);	// �]�w������
	int GetHungyDegree();			//���o��������
	void Setbrain(int nlive);		// �]�w�믫�O
	int Getbrain();					//���o�믫�O��
protected:
	CMovingBitmap Lifeform, LifeBaseMap, HungyDegreeform, HungyDegreeBaseMap,Brainform;	// �ͩR���Ϥ��B���ϻP�ج[�B������
	CAnimation MoribundScrn;
	int x, y;					// �ͩR�楪�W���y��
	int Life, Hungy, brain;
	int Ishurt;
	int InvincibleTime;
};
/////////////////////////////////////////////////////////////////////////////
//�������O
/////////////////////////////////////////////////////////////////////////////
class CRole
{
public:
	CRole();
	int  GetX1();					// ���⥪�W�� x �y��
	int  GetY1();					// ���⥪�W�� y �y��
	int  GetX2();					// ����k�U�� x �y��
	int  GetY2();					// ����k�U�� y �y��
	int GetDir();					//���o�����V
	void SetDir(int nDir);
	void Initialize();				// �]�w���⬰��l��
	void LoadBitmap();				// ���J�ϧ�
	void OnMove();					// ���ʨ���
	void OnShow();					// �N����ϧζK��e��
	void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
	void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
	void SetMovingRight(bool flag); // �]�w�O�_���b���k����
	void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
	void SetSPACE(bool flag);		// �]�w�O�_���U�ť���
	void SetAnimation(bool flag);	// �]�w�}�l����ʵe
	bool GetMovingDown();			// ���o�O�_���b���U����
	bool GetMovingLeft();			// ���o�O�_���b��������
	bool GetMovingRight();			// ���o�O�_���b���k����
	bool GetMovingUp();				// ���o�O�_���b���W����
	bool GetSPACE();				// ���o�O�_���U�ť���
	bool GetAnimation();			//���o�O�_�����ʵe
	void SetXY(int nx, int ny);		// �]�w���⥪�W���y��
	void SetRun(bool run);
	bool GetRun();
	bool HitOther(CMapstuff* stuff,int up);	//�O�_���񦳪���i�QĲ�o
	void RoleItem(CItem* Item);		//����D��P�B
protected:
	CAnimation animationR, animationL, animation, animationU, animationD, Role_Axe_Hack, Role_PickAxe_Hack;		// ���⪺�ʵe
	CMovingBitmap Role_Axe, Role_torchfire, Role_PickAxe, Role_spade;
	CAnimation Role_Pick, Role_Hat, Role_spade_Hack;

	int x, y;					// �H�����W���y��
	bool isMovingDown;			// �O�_���b���U����
	bool isMovingLeft;			// �O�_���b��������
	bool isMovingRight;			// �O�_���b���k����
	bool isMovingUp;			// �O�_���b���W����
	bool isSPACE,IsAnimation ;
	int isSPACEShow;			// 
	bool run;					//�O�_����
	int life;					//�ͩR��
	int dir;					//�����V
	CItem_Data Equipment[3];	//�˳ƪ��D��
private:
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
};
/////////////////////////////////////////////////////////////////////////////
//Bio�ͪ�
/////////////////////////////////////////////////////////////////////////////

class CBio
{
public:
	CBio();
	int  GetX1();					// ���⥪�W�� x �y��
	int  GetY1();					// ���⥪�W�� y �y��
	int  GetX2();					// ����k�U�� x �y��
	int  GetY2();					// ����k�U�� y �y��
	int GetDir();					// ���o�ͪ���V
	void SetDir(int nDir);
	void Initialize();				// �]�w�ͪ�����l��
	virtual void LoadBitmap();		// ���J�ϧ�
	virtual void OnMove();			// ���ʥͪ�
	virtual void OnShow();			// �N�ͪ��ϧζK��e��
	void SetXY(int nx, int ny);		// �]�w�ͪ����W���y��
	void SetRun(bool run);
	bool GetRun();
protected:
	CAnimation animationR, animationL;		// �ͪ����ʵe
	int x, y;					// �ͪ����W���y��
	bool run;					//�O�_����
	int life;					//�ͩR��
	int dir;					//�ͪ���V
	int tiem[2];				//�������~
private:
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
};
///////////////////////
//Clock����
class Clock{
public:
	Clock();
	void LoadBitmap();		// ���J�ϧ�
	void OnMove();			// ����
	void OnShow();			// �N�ϧζK��e��
	void SetTime(int ntime);//�]�w�ɶ�
	int GetTime();			//���o�ɶ�
	int GetDays();			//���o�Ѽ�
private:
	CMovingBitmap timebmp[8];			// �������Ϥ�
	CMovingBitmap dayBD;
	int time;
	bool IsRun;
	int x, y;
	int days;
};
///////////////////////
//Ū�g��CSavefile
class CSavefile{
public:
	CSavefile();
	void LoadBitmap();		// ���J�ϧ�
	void OnMove();			// ����
	void OnShow();			// �N�ϧζK��e��
	void Rfiles(string filename,CRoleState *State, CItem *Item,Clock *clock);			//Ū��
	void Wfiles(string filename, CRoleState *State, CItem *Item, Clock *clock);			//�g��
private:
	CMovingBitmap Rfiles_Img,Wfiles_Img;			// �������Ϥ�
	int x, y;
	int IsShow;
};
///////////////////////
//�Ȱ����CPause Menu
class CPauseMenu{
public:
	CPauseMenu();
	void LoadBitmap();		// ���J�ϧ�
	void OnShow();			// �N�ϧζK��e��
	void SetShow(int flag);
	int  GetShow();
	int IsButton(CPoint P);//����I��Ĳ�o�ƥ�
private:
	CMovingBitmap PauseMenuImp, PauseButtonImp;			// �Ϥ�
	int x, y;
	int IsShow;
};
///////////////////////
//���`���COver Menu
class COverMenu{
public:
	COverMenu();
	void LoadBitmap();		// ���J�ϧ�
	void OnShow(int days);	// �N�ϧζK��e��
	void SetShow(int flag);
	int  GetShow();
	int IsButton(CPoint P); // ����I��Ĳ�o�ƥ�
private:
	CMovingBitmap OverMenuImp, AllBlockImp;			// �Ϥ�
	int x, y;
	int IsShow;
};
///////////////////////
//�p�a��CmimMap
class CmimMap{
public:
	CmimMap();
	void LoadBitmap();		// ���J�ϧ�
	void OnShow(CgameMap *Map,CMapstuff_campfire *fire,CRole *Role);			// �N�ϧζK��e��
	void SetShow(int flag);//�]�w�O�_���
	int  GetShow();			
	int IsButton(CPoint P);//�I��Ĳ�o�ƥ�
private:
	CMovingBitmap mimMapImg, BigMapBack, witer, forest15;			// �Ϥ�
	CMovingBitmap minRole, minFire;
	int x, y;
	int IsShow;
};
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
    protected:
		void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        CMovingBitmap logo;								// �C����logo
		CMovingBitmap BackGround;								// �C�����}�l�I��
		CHelp Help;
};
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        CMovingBitmap	background;	// �I����
		CMovingBitmap   black100,blackO;	//�©]
		CMovingBitmap   text_willdark, text_willsum;//���ܤ�r
		CMovingBitmap   day[8];
		int          picX, picY;
		//�D��&�a��
		CRole Fist_Role;
		CgameMap gamemap;
		////////////////
		//�a�ϤW���ͪ��P�D��
		CMapstuff_tree Tree[90];
		CMapstuff_flint flint[90];
		CMapstuff_Hay Hay[90];
		CMapstuff_Branch Branch[90];
		CMapstuff_stone stone[90];
		CMapstuff_Rabbit Rabbit[90];
		CMapstuff_campfire Campfire[10];
		CMapstuff_BlockDog BlockDog[90];
		CMapstuff_Hat Hat[90];
		CMapstuff_gold Gold[90];
		CMapstuff_Carrot Carrot[90];
		CMapstuff_Berry Berry[90];
		//�����P���A
		CMake Make;
		CItem Item;
		CRoleState RoleState;
		CPoint Mouse;
		Clock  GameClock;
		CSavefile Savefile;
		CPauseMenu PauseMenu;
		CmimMap minMap;
		CSea Sea;
		CText Massage;
		COverMenu GameOver;
		//
};
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        int counter;	// �˼Ƥ��p�ƾ�
		CMovingBitmap DeadMenu;
};
}
