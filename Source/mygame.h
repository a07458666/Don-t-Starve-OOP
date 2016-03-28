/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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

enum AUDIO_ID  				// 定義各種音效的編號
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
	// 地圖素材
	/////////////////////////////////////////////////////////////////////////////
	class CRole;
	class CgameMap;
	class CItem;
	class CMapstuff
	{
	public:
		CMapstuff(int BlockX=0,int BlockY=0);
		bool HitRole(CRole* Role);								// 是否碰到角色
		bool IsAlive();											// 是否活著
		virtual void LoadBitmap()=0;							// 載入圖形
		void OnMove(CgameMap* Map);								// 移動角色(實際遊戲中沒有用到)
		virtual void OnShow()=0;								// 將圖形貼到畫面
		void SetBlockXY(int nx, int ny);						// 設定方格的座標
		void SetIsAlive(bool alive);							// 設定是否活著
		int GetX1();
		int GetX2();
		int GetY1();
		int GetY2();
	protected:
		CMovingBitmap stuffImg;			//圖片
		int x, y;						//角色左上角的座標
		int life;						//生命
		bool is_alive;					// 是否活著
		int BlockX, BlockY;				//方格座標轉地圖用
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
	/////////////////////////////////////////////////////////////////////////////
	// 燧石flint
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_flint: public CMapstuff
	{
	public:
		CMapstuff_flint(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void OnShow();											// 將圖形貼到畫面
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
	/////////////////////////////////////////////////////////////////////////////
	// 樹
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_tree : public CMapstuff
	{
	public:
		CMapstuff_tree(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void OnShow();											// 將圖形貼到畫面
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
	/////////////////////////////////////////////////////////////////////////////
	// 斧頭
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Axes : public CMapstuff
	{
	public:
		CMapstuff_Axes(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void OnShow();											// 將圖形貼到畫面
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
	/////////////////////////////////////////////////////////////////////////////
	// 乾草
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Hay : public CMapstuff
	{
	public:
		CMapstuff_Hay(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void OnShow();											// 將圖形貼到畫面
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
	/////////////////////////////////////////////////////////////////////////////
	// 樹枝Branch
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Branch : public CMapstuff
	{
	public:
		CMapstuff_Branch(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void OnShow();											// 將圖形貼到畫面
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
	/////////////////////////////////////////////////////////////////////////////
	// 石頭stone(6)
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_stone : public CMapstuff
	{
	public:
		CMapstuff_stone(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void OnShow();											// 將圖形貼到畫面
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
	/////////////////////////////////////////////////////////////////////////////
	// 帽子Hat(12)
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Hat : public CMapstuff
	{
	public:
		CMapstuff_Hat(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void OnShow();											// 將圖形貼到畫面
	private:
		int life;
	};
	/////////////////////////////////////////////////////////////////////////////
	// 黃金gold(13)
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_gold : public CMapstuff
	{
	public:
		CMapstuff_gold(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void OnShow();											// 將圖形貼到畫面
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
	/////////////////////////////////////////////////////////////////////////////
	// 紅羅波Carrot(14)
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Carrot : public CMapstuff
	{
	public:
		CMapstuff_Carrot(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void OnShow();											// 將圖形貼到畫面
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
	/////////////////////////////////////////////////////////////////////////////
	// 漿果Berry(15)
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Berry : public CMapstuff
	{
	public:
		CMapstuff_Berry(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void OnShow();											// 將圖形貼到畫面
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
	/////////////////////////////////////////////////////////////////////////////
	// 兔子Rabbit
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_Rabbit : public CMapstuff
	{
	public:
		CMapstuff_Rabbit(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void Initialize();
		void OnShow();											// 將圖形貼到畫面
		void OnMove(CgameMap *Map);
		void avoidMove(CgameMap *Map, CRole *Role);
	protected:
		CAnimation animationR, animationL;
		CMovingBitmap Rstop, Lstop;
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
		int MoveTimes, dir, olddir;
		int nX, nY;
		int life;
	};
	/////////////////////////////////////////////////////////////////////////////
	// 黑狗blockdog
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_BlockDog :public CMapstuff
	{
	public:
		CMapstuff_BlockDog(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void Initialize();										//初始化
		void OnShow();											// 將圖形貼到畫面
		void OnMove(CgameMap *Map);								//一般移動
		void TraceMove(CgameMap *Map, CRole *Role);				//追人模式
		void Setfight(bool is);									//判斷攻擊距離
		bool GetHit();											//是否擊中
		int  Getlife();											//取得黑狗生命值
		int  OnHit();											//受到攻擊
	protected:
		CAnimation animationR, animationL;						//左右移動動畫
		CAnimation Rstop, Lstop,fightR,fightL;					//左右停頓、攻擊動畫
	private:
		int MoveTimes, MoveTimesHit, dir, olddir;				//移動時間、攻擊時間、方向、前一次方向
		int nX, nY;												//座標
		int life;												//狗的生命
		bool Is_fight;											//是否為攻擊狀態
		bool Hit;												//是否為受傷狀態
	};
	/////////////////////////////////////////////////////////////////////////////
	// 營火campfire
	/////////////////////////////////////////////////////////////////////////////
	class CMapstuff_campfire : public CMapstuff
	{
	public:
		CMapstuff_campfire(int BlockX = 0, int BlockY = 0);
		void LoadBitmap();										// 載入圖形
		void Initialize();
		void OnShow();											// 將圖形貼到畫面
		void SetCampfire(int x, int y, CgameMap *Map);			//將營火設定至地圖上
		void OnMove(CgameMap *Map);
		void SetTime(bool flag);								//設定白天晚上營火圖片
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
		int inX, inY;											//被創造的位置
		bool time;
		CMovingBitmap BCampFire;								//晚上的螢火圖
	};
	/////////////////////////////////////////////////////////////////////////////
	// 地圖類別
	/////////////////////////////////////////////////////////////////////////////
	class CgameMap{
	public:
		CgameMap();
		void  Initialize();				// 設定地圖為初始值
		void LoadBitmap();
		void OnShow();
		void OnMove(CRole *role, CMapstuff* tree, CMapstuff* stone);
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		int GetX1();
		int GetX2();
		int GetY1();
		int GetY2();
		void SetHit(bool hit);			//設定碰撞
		bool GetHit();					//取得碰撞
		void SetXY(int nx, int ny);		// 設定擦子左上角座標
		void SetRun(bool run);			//設定是否可以移動
		bool GetRun();					//取得是否可以移動
		int GetMH();					//取得地圖方格高度
		int GetMW();					//取得地圖方格高度
		int Loadstuff(int x, int y);				//載入物體
		int Onshowstuff();				//顯示物體
		int GetMap_G(int x, int y);
		~CgameMap();

	private:
		CMovingBitmap ground,stone,witer,biggrass;	//各類地形
		CMovingBitmap forest11, forest12, forest13, forest14, forest15, forest16, forest17, forest18, forest19;
		int map[20][20],map_stuff[40][40];		//地圖與道具陣列
		int x, y;
		const int MW, MH;			//單格長寬
		CAnimation animationMap;	// 不動的動畫
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		bool run;					//是否移動
		bool Hit;					//發生碰撞
	};
/////////////////////////////////////////////////////////////////////////////
// 海
/////////////////////////////////////////////////////////////////////////////
	class CSea{
	public:
		CSea();
		void  Initialize();				// 設定初始值
		void LoadBitmap();
		void OnShow();
		void OnMove(CgameMap *Map);
		void SetXY(int nx, int ny);		// 設定左上角座標
		void SetRun(bool run);			//設定是否可以移動
		bool GetRun();					//取得是否可以移動
	private:
		CMovingBitmap SeaImage;
		int x, y;
		bool run;					//是否移動
		int UpDown;					//飄移方向
		int loop;					
	};
/////////////////////////////////////////////////////////////////////////////
// 操作說明、玩法說明、密技說明
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
		int  IsMouse(CPoint *point);	//回傳誰被碰
	private:
		int x, y;
		int run,oldrun;					//為動作的物件編號
		int movenum1,movenum2, movenum3;//物件的各移動值
		CMovingBitmap ControlImg, GoalImg,ItemImg;//操作、遊戲說明道具
};
/////////////////////////////////////////////////////////////////////////////
// 文字方框
/////////////////////////////////////////////////////////////////////////////
class CText
{
	public:
		CText();
		void OnMove();
		void OnShow();
		void LoadBitmap();
		void SetIsShow(bool IsShow);					//是否顯示
		bool GetIsShow();								//取得是否顯示
		void SetStoryVariable(int StoryVariable);		//設定劇情章節
		int  GetStoryVariable();						//取得劇情章節
		void SetText(string Text, int Txtscale);		//載入文字
		void Readfile(string filename, int RowNum, int Txtscale);//讀字檔
		void SetKillAnimal(int ID, int num);			//紀錄擊殺數
		int  GetKillAniml(int ID);						//取得擊殺數
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
// 製造列
/////////////////////////////////////////////////////////////////////////////
class CMake
{
public:
	CMake();
	int  GetX1();					// 製造左上角 x 座標
	int  GetY1();					// 製造左上角 y 座標
	int  GetX2();					// 製造右下角 x 座標
	int  GetY2();					// 製造右下角 y 座標
	void Initialize();							// 設定製造為初始值
	void LoadBitmap();							// 載入圖形
	void OnShow();								// 將製造列圖形貼到畫面
	void SetXY(int nx, int ny);					// 設定擦子左上角座標
	int OnMouseDown();							//滑鼠點擊
	void SetMake_AxesOnSow(int flag);			//設定顯示斧頭介面
	int  GetMake_AxesOnSow();					//取得顯示斧頭介面
	void SetMake_torchfireOnSow(int flag);		//設定顯示火把介面
	int  GetMake_torchfireOnSow();				//取得顯示火把介面
	void SetMake_campfireOnSow(int flag);		//設定顯示營火介面
	int  GetMake_campfireOnSow();				//取得顯示營火介面
	void SetMake_spadeShow(int flag);			//設定顯示鏟子介面
	int  GetMake_spadeShow();					//取得顯示鏟子介面
	void SetMake_pickaxeShow(int flag);			//設定顯示鋤頭介面
	int  GetMake_pickaxeShow();					//取得顯示鋤頭介面
	void SetMake_hud_tools_Show(int flag);		//設定顯示類別介面
	int  GetMake_hud_tools_Show();				//取得顯示類別介面
	bool BuildHit(int x, int y);				//建造觸發事件
	bool MakeformHit(int x, int y);				//展開觸發事件
	bool Enough(CItem item,int ID1,int amount1,int ID2,int amount2);//是否可以建造
protected:
	CMovingBitmap Makeform, MakeBlack[10],
				  AxesYesform, AxesNoform,
				  torchfireYesform, torchfireNoform,
				  campfireYesform, campfireNoform,
				  spadeYesform,spadeNoform,
				  pickaxeYesform,pickaxeNoform,
				  hud_tools_fire, hud_tools;		// 製作列的動畫
	int x, y;									// 製作列左上角座標
	int AxesShow, campfireShow, torchfireShow, hud_tools_Show, spadeShow,pickaxeShow;
};
/////////////////////////////////////////////////////////////////////////////
// C道具
/////////////////////////////////////////////////////////////////////////////
class CItem_Data
{
public:
	CItem_Data();
	void Initialize();				// 設定製造為初始值
	void SetId(int Id);				//設定道具邊號
	int  GetId();					//取得道具編號
	void Setamount(int amount);		//設定數量
	int Getamount();				//取得數量
protected:
	int x, y;					// 製作列左上角座標
	int ItemId;					//道具編號
	int amount;					//道具數量
private:	
};

/////////////////////////////////////////////////////////////////////////////
// 道具欄
/////////////////////////////////////////////////////////////////////////////

class CItem
{
public:
	CItem();
	int  GetX1();					// 製造左上角 x 座標
	int  GetY1();					// 製造左上角 y 座標
	int  GetX2();					// 製造右下角 x 座標
	int  GetY2();					// 製造右下角 y 座標
	void Initialize();				// 設定製造為初始值
	void LoadBitmap();				// 載入圖形
	void OnShow();					// 將製造列圖形貼到畫面
	void SetXY(int nx, int ny);		// 設定擦子左上角座標
	bool GetItem(int ItemId,int amount);//獲得道具
	bool LostItem(int ItemId,int amount);//失去
	bool ChaseMouse(int mX, int mY);//拿起道具
	void SetChoiceItem(int ID);//設定鼠標上的道具
	int  GetChoiceItem();//取得鼠標上的道具
	void SetfireYes(int flog);//設定顯示在鼠邊旁的文字
	CItem_Data GetHaveItem(int ID);//取得道具欄資料
protected:
	CMovingBitmap Itemform, Item_flint, Item_tree,// 道具列的動畫
		Item_Axes, Item_Hay, Item_Branch,
		Item_stone, Item_Rabbit, Item_torchfire, Item_RabbitDead,
		Item_spade, Item_pickaxe,
		Item_Hat, Item_gold,
		Item_Carrot, Item_Berry,
		Item_Honey;
	CMovingBitmap face_roast, face_drop;
	int x, y;											// 製作列左上角座標
	CItem_Data Have_Item[19];
private:
	CInteger ItemAmount;
	int ChoiceItem, fireYes;
};

/////////////////////////////////////////////////////////////////////////////
// 角色狀態欄
/////////////////////////////////////////////////////////////////////////////
class CRoleState
{
public:
	CRoleState();
	void Initialize();				// 設定初始值
	void LoadBitmap();				// 載入圖形
	void OnShow();					// 將狀態欄圖形貼到畫面
	void OnMove(int time, CItem Item,bool fire);//狀態變化
	void SetLife(int nlife);		// 設定生命
	int GetLife();					//取得生命
	void SetHungyDegree(int nlive);	// 設定飽食度
	int GetHungyDegree();			//取得飽食度欄
	void Setbrain(int nlive);		// 設定精神力
	int Getbrain();					//取得精神力欄
protected:
	CMovingBitmap Lifeform, LifeBaseMap, HungyDegreeform, HungyDegreeBaseMap,Brainform;	// 生命的圖片、底圖與框架、飽食度
	CAnimation MoribundScrn;
	int x, y;					// 生命欄左上角座標
	int Life, Hungy, brain;
	int Ishurt;
	int InvincibleTime;
};
/////////////////////////////////////////////////////////////////////////////
//角色類別
/////////////////////////////////////////////////////////////////////////////
class CRole
{
public:
	CRole();
	int  GetX1();					// 角色左上角 x 座標
	int  GetY1();					// 角色左上角 y 座標
	int  GetX2();					// 角色右下角 x 座標
	int  GetY2();					// 角色右下角 y 座標
	int GetDir();					//取得角色方向
	void SetDir(int nDir);
	void Initialize();				// 設定角色為初始值
	void LoadBitmap();				// 載入圖形
	void OnMove();					// 移動角色
	void OnShow();					// 將角色圖形貼到畫面
	void SetMovingDown(bool flag);	// 設定是否正在往下移動
	void SetMovingLeft(bool flag);	// 設定是否正在往左移動
	void SetMovingRight(bool flag); // 設定是否正在往右移動
	void SetMovingUp(bool flag);	// 設定是否正在往上移動
	void SetSPACE(bool flag);		// 設定是否按下空白鍵
	void SetAnimation(bool flag);	// 設定開始播放動畫
	bool GetMovingDown();			// 取得是否正在往下移動
	bool GetMovingLeft();			// 取得是否正在往左移動
	bool GetMovingRight();			// 取得是否正在往右移動
	bool GetMovingUp();				// 取得是否正在往上移動
	bool GetSPACE();				// 取得是否按下空白鍵
	bool GetAnimation();			//取得是否結束動畫
	void SetXY(int nx, int ny);		// 設定角色左上角座標
	void SetRun(bool run);
	bool GetRun();
	bool HitOther(CMapstuff* stuff,int up);	//是否附近有物件可被觸發
	void RoleItem(CItem* Item);		//角色道具同步
protected:
	CAnimation animationR, animationL, animation, animationU, animationD, Role_Axe_Hack, Role_PickAxe_Hack;		// 角色的動畫
	CMovingBitmap Role_Axe, Role_torchfire, Role_PickAxe, Role_spade;
	CAnimation Role_Pick, Role_Hat, Role_spade_Hack;

	int x, y;					// 人物左上角座標
	bool isMovingDown;			// 是否正在往下移動
	bool isMovingLeft;			// 是否正在往左移動
	bool isMovingRight;			// 是否正在往右移動
	bool isMovingUp;			// 是否正在往上移動
	bool isSPACE,IsAnimation ;
	int isSPACEShow;			// 
	bool run;					//是否移動
	int life;					//生命值
	int dir;					//角色方向
	CItem_Data Equipment[3];	//裝備的道具
private:
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
};
/////////////////////////////////////////////////////////////////////////////
//Bio生物
/////////////////////////////////////////////////////////////////////////////

class CBio
{
public:
	CBio();
	int  GetX1();					// 角色左上角 x 座標
	int  GetY1();					// 角色左上角 y 座標
	int  GetX2();					// 角色右下角 x 座標
	int  GetY2();					// 角色右下角 y 座標
	int GetDir();					// 取得生物方向
	void SetDir(int nDir);
	void Initialize();				// 設定生物為初始值
	virtual void LoadBitmap();		// 載入圖形
	virtual void OnMove();			// 移動生物
	virtual void OnShow();			// 將生物圖形貼到畫面
	void SetXY(int nx, int ny);		// 設定生物左上角座標
	void SetRun(bool run);
	bool GetRun();
protected:
	CAnimation animationR, animationL;		// 生物的動畫
	int x, y;					// 生物左上角座標
	bool run;					//是否移動
	int life;					//生命值
	int dir;					//生物方向
	int tiem[2];				//掉落物品
private:
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
};
///////////////////////
//Clock時鐘
class Clock{
public:
	Clock();
	void LoadBitmap();		// 載入圖形
	void OnMove();			// 移動
	void OnShow();			// 將圖形貼到畫面
	void SetTime(int ntime);//設定時間
	int GetTime();			//取得時間
	int GetDays();			//取得天數
private:
	CMovingBitmap timebmp[8];			// 時鐘的圖片
	CMovingBitmap dayBD;
	int time;
	bool IsRun;
	int x, y;
	int days;
};
///////////////////////
//讀寫檔CSavefile
class CSavefile{
public:
	CSavefile();
	void LoadBitmap();		// 載入圖形
	void OnMove();			// 移動
	void OnShow();			// 將圖形貼到畫面
	void Rfiles(string filename,CRoleState *State, CItem *Item,Clock *clock);			//讀檔
	void Wfiles(string filename, CRoleState *State, CItem *Item, Clock *clock);			//寫檔
private:
	CMovingBitmap Rfiles_Img,Wfiles_Img;			// 時鐘的圖片
	int x, y;
	int IsShow;
};
///////////////////////
//暫停菜單CPause Menu
class CPauseMenu{
public:
	CPauseMenu();
	void LoadBitmap();		// 載入圖形
	void OnShow();			// 將圖形貼到畫面
	void SetShow(int flag);
	int  GetShow();
	int IsButton(CPoint P);//菜單點擊觸發事件
private:
	CMovingBitmap PauseMenuImp, PauseButtonImp;			// 圖片
	int x, y;
	int IsShow;
};
///////////////////////
//死亡菜單COver Menu
class COverMenu{
public:
	COverMenu();
	void LoadBitmap();		// 載入圖形
	void OnShow(int days);	// 將圖形貼到畫面
	void SetShow(int flag);
	int  GetShow();
	int IsButton(CPoint P); // 菜單點擊觸發事件
private:
	CMovingBitmap OverMenuImp, AllBlockImp;			// 圖片
	int x, y;
	int IsShow;
};
///////////////////////
//小地圖CmimMap
class CmimMap{
public:
	CmimMap();
	void LoadBitmap();		// 載入圖形
	void OnShow(CgameMap *Map,CMapstuff_campfire *fire,CRole *Role);			// 將圖形貼到畫面
	void SetShow(int flag);//設定是否顯示
	int  GetShow();			
	int IsButton(CPoint P);//點擊觸發事件
private:
	CMovingBitmap mimMapImg, BigMapBack, witer, forest15;			// 圖片
	CMovingBitmap minRole, minFire;
	int x, y;
	int IsShow;
};
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作
    protected:
		void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        CMovingBitmap logo;								// 遊戲的logo
		CMovingBitmap BackGround;								// 遊戲的開始背景
		CHelp Help;
};
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        CMovingBitmap	background;	// 背景圖
		CMovingBitmap   black100,blackO;	//黑夜
		CMovingBitmap   text_willdark, text_willsum;//提示文字
		CMovingBitmap   day[8];
		int          picX, picY;
		//主角&地圖
		CRole Fist_Role;
		CgameMap gamemap;
		////////////////
		//地圖上的生物與道具
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
		//介面與狀態
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
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        int counter;	// 倒數之計數器
		CMovingBitmap DeadMenu;
};
}
