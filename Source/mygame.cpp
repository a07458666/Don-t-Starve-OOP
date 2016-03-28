/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
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
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include<fstream>
#include<sstream>

namespace game_framework
{
	////////////////////////////////////////////////////////////////////////////
	// 地圖
	////////////////////////////////////////////////////////////////////////////

	CgameMap::CgameMap()
		:x(-10 * 256 + SIZE_X / 2 + 128 - 10), y(-10 * 256 + SIZE_Y / 2 + 128), MW(256), MH(256)
	{
		Hit = false;
		run = true;								//地圖是否移動
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
		int map_init[20][20] = {//地圖資料
			{ 15, 15, 15, 15, 15,  2,  2,  2, 15, 15, 15,  2,  2,  2,  2,  2,  2,  2,  2,  2 },
			{ 15, 15, 15, 15, 15, 15,  2, 15, 15, 15, 15, 15,  2,  2,  2,  2,  2,  2,  2,  2 },
			{ 15, 15, 15, 15, 15, 15,  2, 15, 15, 15, 15, 15, 15,  2,  2,  2,  2,  2,  2,  2 },
			{ 15, 15, 15, 15, 15, 15,  2, 15, 15, 15, 15, 15, 15, 15,  2,  2,  2,  2,  2,  2 },
			{ 15, 15, 15, 15, 15,  2,  2,  2, 15, 15, 15, 15, 15, 15,  2,  2,  2,  2,  2,  2 },
			{  2,  2, 15, 15, 15, 15,  2, 15, 15, 15, 15, 15, 15, 15,  2, 15,  2,  2,  2,  2 },
			{ 15,  2,  2, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  2,  2, 15, 15,  2,  2,  2 },
			{ 15, 15,  2, 15, 15, 15, 15, 15, 15, 15, 15,  2, 15,  2,  2, 15, 15, 15,  2,  2 },
			{  2, 15,  2, 15, 15, 15, 15, 15, 15, 15,  2,  2, 15,  2, 15, 15, 15,  2,  2,  2 },
			{  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 15, 15, 15,  2,  2,  2 },
			{  2, 15, 15, 15,  2,  2,  2, 15, 15,  2,  2, 15,  2, 15, 15, 15, 15,  2,  2,  2 },
			{ 15, 15, 15, 15, 15,  2, 15, 15, 15, 15,  2, 15, 15, 15, 15, 15,  2,  2,  2,  2 },
			{ 15, 15, 15, 15, 15, 15, 15, 15,  2,  2,  2, 15, 15, 15, 15,  2,  2, 15, 15,  2 },
			{ 15, 15, 15, 15, 15, 15, 15,  2,  2, 15, 15, 15, 15, 15,  2,  2, 15, 15, 15,  2 },
			{ 15, 15, 15, 15, 15, 15, 15,  2,  2, 15, 15, 15, 15, 15,  2, 15, 15, 15, 15, 15 },
			{ 15, 15,  2, 15, 15, 15,  2,  2, 15, 15, 15, 15, 15, 15,  2, 15, 15, 15, 15, 15 },
			{ 15,  2,  2, 15, 15, 15,  2,  2, 15, 15, 15, 15,  2,  2,  2, 15, 15, 15, 15, 15 },
			{  2,  2, 15, 15, 15, 15, 15,  2, 15,  2, 15, 15,  2,  2, 15, 15, 15, 15, 15, 15 },
			{  2,  2, 15, 15, 15, 15, 15,  2,  2,  2,  2, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
			{  2,  2,  2, 15, 15, 15, 15, 15,  2,  2,  2,  2, 15, 15, 15, 15, 15, 15, 15, 15 }
		};
		int map_stuffIn[40][40] = {//改為隨機生成
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 1, 6, 4, 5, 5, 1, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 2, 2, 0, 0, 0, 0, 0, 1, 1, 1, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 2, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
		};
		for (int i = 0; i < 20; ++i)				//寫入地圖資料
			for (int j = 0; j < 20; ++j)
				map[i][j] = map_init[i][j];
		srand(time(NULL));
		for (int i = 1; i < 40; ++i)				//寫入地圖物件資料
			for (int j = 0; j < 40; ++j)
				if(1)map_stuff[i][j] = rand() % 20;
				else map_stuffIn[i][j];
	}
	void CgameMap::Initialize(){				// 設定地圖為初始值
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
		x = -10 * 256 + SIZE_X / 2 + 128 - 10;
		y = -10 * 256 + SIZE_Y / 2 + 128;
	}
	void CgameMap::LoadBitmap(){
		witer.LoadBitmap(IDB_G_Grass,RGB(255,255,255));
		forest15.LoadBitmap(IDB_map_forest15, RGB(255, 255, 255));

		biggrass.LoadBitmap(IDB_G_BigGress,0xffffff);			//舊地圖

		forest11.LoadBitmap(IDB_map_forest11, RGB(255, 255, 255));//邊界圖
		forest12.LoadBitmap(IDB_map_forest12, RGB(255, 255, 255));
		forest13.LoadBitmap(IDB_map_forest13, RGB(255, 255, 255));
		forest14.LoadBitmap(IDB_map_forest14, RGB(255, 255, 255));
		forest16.LoadBitmap(IDB_map_forest16, RGB(255, 255, 255));
		forest17.LoadBitmap(IDB_map_forest17, RGB(255, 255, 255));
		forest18.LoadBitmap(IDB_map_forest18, RGB(255, 255, 255));
		forest19.LoadBitmap(IDB_map_forest19, RGB(255, 255, 255));
	}
	void CgameMap::SetRun(bool Nrun){	//設定是否移動
		run = Nrun;
	}
	bool CgameMap::GetRun(){			//取得是否移動
		return run;
	}
	void CgameMap::OnMove(CRole *role, CMapstuff* tree,CMapstuff* stone)
	{	
		int oldx = x, oldy = y;//處存原本位置
		const int STEP_SIZE = 8;
		////////////////////////////////////////////////////////////////////////////
		if (isMovingLeft)//x軸移動
			x += STEP_SIZE;
		if (isMovingRight)
			x -= STEP_SIZE;
		if (role->GetX1() <= x || role->GetX2() >= x + MW * 20){
			x = oldx;
		}
		////////////////////////////////////////////////////////////////////////////
		if (isMovingUp){//y軸移動
			y += STEP_SIZE;
		}
		if (isMovingDown){
			y -= STEP_SIZE;
		}
		if (role->GetY1()<=y || role->GetY2()>=y+20*MH){
			y = oldy;
		}
		/////////////////////////////樹的碰撞//////////////////////////////////////////
		for (int i = 0; i < 90; i++){
			tree->OnMove(this);
				if (tree->HitRole(role)){
					x = oldx;
					y = oldy;
				}
				tree++;
		}
		//////////////////////////////石頭的碰撞////////////////////////////////////////
		for (int i = 0; i < 90; i++){
			stone->OnMove(this);
			
			if (role->HitOther(stone,15)){
				x = oldx;
				y = oldy;
			}
			stone++;
		}
		return ;
	}
	void CgameMap::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}
	void CgameMap::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}
	void CgameMap::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}
	void CgameMap::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}
	void CgameMap::OnShow()
	{
		for (int i = 0; i < 20; ++i){				//顯示地圖資料
			for (int j = 0; j < 20; ++j){
				if (map[i][j]==2){
					witer.SetTopLeft(x + (MW*j) - MW, y + (MH*i) - MH);
					witer.ShowBitmap();
				}
			}
		}
		for (int i = 0; i < 20; ++i){				//顯示地圖資料
			for (int j = 0; j < 20; ++j){
				if (map[i][j] == 15){
					forest15.SetTopLeft(x + (MW*j) - MW, y + (MH*i) - MH);
					forest15.ShowBitmap();
				}
			}
		}
	}
	void CgameMap::SetXY(int nx, int ny)
	{
		x = nx;
		y = ny;
	}
	int CgameMap::GetX1(){
		return x;
	}
	int CgameMap::GetY1(){
		return y;
	}
	int CgameMap::GetX2(){
		return x+MW*20;
	}
	int CgameMap::GetY2(){
		return y+MH*20;
	}
	void CgameMap::SetHit(bool OnHit){
		Hit = OnHit;
	}
	bool CgameMap::GetHit(){
		return Hit;
	}
	int CgameMap::GetMW(){
		return MW;
	}
	int CgameMap::GetMH(){
		return MH;
	}
	int CgameMap::Loadstuff(int x, int y){
		return map_stuff[y][x];
	}
	int CgameMap::GetMap_G(int x, int y){
		return map[y][x];
	}
	CgameMap::~CgameMap(){

	}
/////////////////////////////////////////////////////////////////////////////
//海
/////////////////////////////////////////////////////////////////////////////
	CSea::CSea(){
		x = 0;
		y = 0;
		run = true;
		UpDown = 25;
		loop = 0;
	}
	void CSea::Initialize(){				// 設定初始值
		x = 0;
		y = 0;
	}
	void CSea::LoadBitmap(){
		SeaImage.LoadBitmap(IDB_Sea, RGB(255, 255, 255));
	}
	void CSea::OnShow(){
		int Sign = 1;
		if (UpDown == 0)loop = 1;
		else if (UpDown == 25)loop = 0;
		if (loop == 1)UpDown++;
		else UpDown--;

		for (int i = 0; i < 70; i++){
			if ((i % 2) == 1)Sign = 1;
			else Sign = -1;
			for (int j = 0; j < 70; j++){
				SeaImage.SetTopLeft(x + (i % 2) * 50 + SeaImage.Width()*j, y + 100 * i + Sign*UpDown);
				SeaImage.ShowBitmap();
			}
		}
	}
	void CSea::OnMove(CgameMap *Map){
		x = Map->GetX1()-2*Map->GetMW();
		y = Map->GetY1()-2*Map->GetMH();
	}
	void CSea::SetXY(int nx, int ny){		// 設定左上角座標
		x = nx;
		y = ny;
	}
	void CSea::SetRun(bool Nrun){			//設定是否可以移動
		run = Nrun;
	}
	bool CSea::GetRun(){					//取得是否可以移動
		return run;
	}
/////////////////////////////////////////////////////////////////////////////
//地圖素材
/////////////////////////////////////////////////////////////////////////////
	CMapstuff::CMapstuff(int InBlockX, int InBlockY) 
	{
		BlockX = InBlockX;
		BlockY = InBlockY;
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff::OnMove(CgameMap *Map)
	{	
		if (IsAlive()){
			x = Map->GetX1() + BlockX*Map->GetMW()/2;					//地圖位置座標
			y = Map->GetY1() + BlockY*Map->GetMH()/2;					//地圖位置座標
		}
	}
	void CMapstuff::SetBlockXY(int nx, int ny)//載入地圖位置座標
	{
		BlockX = nx;
		BlockY = ny;
	}
	void CMapstuff::SetIsAlive(bool alive){
		is_alive = alive;
	}
	bool CMapstuff::IsAlive()
	{
		return is_alive;
	}
	bool CMapstuff::HitRectangle(int Rx1, int Ry1, int Rx2, int Ry2)
	{
		int Tx1 = x + stuffImg.Width()/3/2;				// 物件的左上角x座標
		int Ty1 = y + stuffImg.Height()/2*9/10 ;				// 物件的左上角y座標
		int Tx2 = x + stuffImg.Width()*2/3/2;				// 物件的右下角x座標
		int Ty2 = y + stuffImg.Height()/2;				// 物件的右下角y座標
		Ry1 = Ry2 - 50;
		Rx1 += 20;
		Rx2 -= 20;
		//
		// 檢測球的矩形與參數矩形是否有交集
		//
		return (Rx2 >= Tx1 && Rx1 <= Tx2 && Ry2 >= Ty1 && Ry1 <= Ty2 && is_alive);
	}
	int CMapstuff::GetX1(){
		return x;
	}
	int CMapstuff::GetY1(){
		return y;
	}
	int CMapstuff::GetX2(){
		return x+stuffImg.Width()/2;
	}
	int CMapstuff::GetY2(){
		return y+stuffImg.Height()/2;
	}
	bool CMapstuff::HitRole(CRole* Role){
		return HitRectangle(Role->GetX1(), Role->GetY1(),
			Role->GetX2(), Role->GetY2());
	}
/////////////////////////////////////////////////////////////////////////////
//燧石
/////////////////////////////////////////////////////////////////////////////
	CMapstuff_flint::CMapstuff_flint(int InBlockX, int InBlockY) :CMapstuff(InBlockX,InBlockY)
	{
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff_flint::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_flint, 0xffffff);
	}
	void CMapstuff_flint::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			stuffImg.SetTopLeft(x, y);
			stuffImg.ShowBitmap(0.8);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//樹
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_tree::CMapstuff_tree(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff_tree::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_tree, 0xffffff);
	}
	void CMapstuff_tree::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			stuffImg.SetTopLeft(x, y);
			stuffImg.ShowBitmap(0.5);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//斧頭
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_Axes::CMapstuff_Axes(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff_Axes::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_Axes, 0xffffff);
	}
	void CMapstuff_Axes::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			stuffImg.SetTopLeft(x, y);
			stuffImg.ShowBitmap(1);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//Hay乾草
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_Hay::CMapstuff_Hay(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff_Hay::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_MapHay, 0xffffff);
	}
	void CMapstuff_Hay::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			stuffImg.SetTopLeft(x, y);
			stuffImg.ShowBitmap(0.5);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//Branch樹枝
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_Branch::CMapstuff_Branch(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff_Branch::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_map_branch, 0xffffff);
	}
	void CMapstuff_Branch::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			stuffImg.SetTopLeft(x, y);
			stuffImg.ShowBitmap(0.5);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//Hat帽子
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_Hat::CMapstuff_Hat(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff_Hat::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_item_Hat, 0xffffff);
	}
	void CMapstuff_Hat::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			stuffImg.SetTopLeft(x, y);
			stuffImg.ShowBitmap();
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//gold黃金
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_gold::CMapstuff_gold(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff_gold::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_item_gold, 0xffffff);
	}
	void CMapstuff_gold::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			stuffImg.SetTopLeft(x, y);
			stuffImg.ShowBitmap(0.5);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//Carrot紅羅波
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_Carrot::CMapstuff_Carrot(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff_Carrot::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_map_Carrot, 0xffffff);
	}
	void CMapstuff_Carrot::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			stuffImg.SetTopLeft(x, y);
			stuffImg.ShowBitmap();
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//Berry漿果
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_Berry::CMapstuff_Berry(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff_Berry::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_item_Berry, 0xffffff);
	}
	void CMapstuff_Berry::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			stuffImg.SetTopLeft(x, y);
			stuffImg.ShowBitmap(0.8);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//石頭stone
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_stone::CMapstuff_stone(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		x = 0;
		y = 0;
		is_alive = true;
	}
	void CMapstuff_stone::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_map_stone, 0xffffff);
	}
	void CMapstuff_stone::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			stuffImg.SetTopLeft(x, y);
			stuffImg.ShowBitmap(0.5);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//兔子Rabbit
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_Rabbit::CMapstuff_Rabbit(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		Initialize();
	}
	void CMapstuff_Rabbit::LoadBitmap(){
		animationL.SetDelayCount(5);
		animationR.SetDelayCount(5);

		Rstop.LoadBitmap(IDB_Map_rabbitR1, RGB(255, 255, 255));
		Lstop.LoadBitmap(IDB_Map_rabbitL1, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_rabbitR1, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_rabbitR2, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_rabbitR3, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_rabbitR4, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_rabbitR3, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_rabbitR2, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_rabbitR1, RGB(255, 255, 255));

		animationL.AddBitmap(IDB_Map_rabbitL1, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_Map_rabbitL2, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_Map_rabbitL3, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_Map_rabbitL4, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_Map_rabbitL3, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_Map_rabbitL2, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_Map_rabbitL1, RGB(255, 255, 255));
		stuffImg.LoadBitmap(IDB_Map_rabbitL1, 0xffffff);
	}
	void CMapstuff_Rabbit::Initialize(){
		x = 0;
		y = 0;
		is_alive = true;
		MoveTimes = 4;
		dir = 0;
		nX = 0;
		nY = 0;
		olddir = 0;
		life = 5;
	}
	void CMapstuff_Rabbit::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){				//移動方向
			if (dir == 1 || dir == 4){
				animationL.SetTopLeft(x, y);
				animationL.OnShow();
			}
			else if (dir == 2 || dir == 3){
				animationR.SetTopLeft(x, y);
				animationR.OnShow();
			}
			else {					//如果沒動
				if (olddir == 2 || olddir == 3){
					Rstop.SetTopLeft(x, y);
					Rstop.ShowBitmap();
				}
				else
				{
					Lstop.SetTopLeft(x, y);
					Lstop.ShowBitmap();
				}
			}
		}

	}
	void CMapstuff_Rabbit::OnMove(CgameMap *Map){
		int oldnx = nX, oldny = nY;
		int STEP_SIZE = 4;
		animationR.OnMove();
		animationL.OnMove();
		//移動模式
		if (MoveTimes == 0){
			if (dir <= 4)olddir = dir;
			dir = rand() % 20 + 1;
			MoveTimes = 16;
		}
		else MoveTimes--;
		//移動
		if (dir == 1 && is_alive)
		{
			nX -= STEP_SIZE;
			nY -= STEP_SIZE;
		}
		if (dir == 2 && is_alive)
		{
			nX += STEP_SIZE;
			nY += STEP_SIZE;
		}
		if (dir == 3 && is_alive)
		{
			nX += STEP_SIZE;
			nY -= STEP_SIZE;
		}
		if (dir == 4 && is_alive)
		{
			nX -= STEP_SIZE;
			nY += STEP_SIZE;
		}
		//出界判斷
		x = nX + Map->GetX1() + BlockX*Map->GetMW() / 2;
		y = nY + Map->GetY1() + BlockY*Map->GetMH() / 2;
		if (x<Map->GetX1() + 100 || x>Map->GetX2() - 100)nX = oldnx;
		if (y<Map->GetY1() + 100 || y>Map->GetY2() - 100)nY = oldny;
		x = nX + Map->GetX1() + BlockX*Map->GetMW() / 2;
		y = nY + Map->GetY1() + BlockY*Map->GetMH() / 2;

	}
	void CMapstuff_Rabbit::avoidMove(CgameMap *Map, CRole *Role){
		int STEP_SIZE = 6;
		int oldnx=nX, oldny=nY;
		animationR.OnMove();
		animationL.OnMove();
		if (Role->GetX1() > x && Role->GetY1() > y)dir = 1;
		else if (Role->GetX1() < x && Role->GetY1() < y)dir = 2;
		else if (Role->GetX1() > x && Role->GetY1() < y)dir = 4;
		else if (Role->GetX1() < x && Role->GetY1() > y)dir = 3;
		if (dir == 1 && is_alive)
		{
			nX -= STEP_SIZE;
			nY -= STEP_SIZE;
		}
		if (dir == 2 && is_alive)
		{
			nX += STEP_SIZE;
			nY += STEP_SIZE;
		}
		if (dir == 3 && is_alive)
		{
			nX += STEP_SIZE;
			nY -= STEP_SIZE;
		}
		if (dir == 4 && is_alive)
		{
			nX -= STEP_SIZE;
			nY += STEP_SIZE;
		}
		x = nX + Map->GetX1() + BlockX*Map->GetMW() / 2;
		y = nY + Map->GetY1() + BlockY*Map->GetMH() / 2;
		if (x<Map->GetX1() + 100 || x>Map->GetX2() - 100)nX = oldnx;
		if (y<Map->GetY1() + 100 || y>Map->GetY2() - 100)nY = oldny;
		x = nX + Map->GetX1() + BlockX*Map->GetMW() / 2;
		y = nY + Map->GetY1() + BlockY*Map->GetMH() / 2;
	}
	/////////////////////////////////////////////////////////////////////////////
	//黑狗BlockDog
	////////////////////////////////////////////////////////////////////////////
	CMapstuff_BlockDog::CMapstuff_BlockDog(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		Initialize();
	}
	void CMapstuff_BlockDog::LoadBitmap(){
		animationL.SetDelayCount(1);
		animationR.SetDelayCount(1);
		Rstop.SetDelayCount(4);
		Lstop.SetDelayCount(4);
		fightL.SetDelayCount(3);
		fightR.SetDelayCount(3);

		Rstop.AddBitmap(IDB_Map_dogR1, RGB(255, 255, 255));
		Rstop.AddBitmap(IDB_Map_dogR2, RGB(255, 255, 255));
		Rstop.AddBitmap(IDB_Map_dogR3, RGB(255, 255, 255));
		Rstop.AddBitmap(IDB_Map_dogR4, RGB(255, 255, 255));
		Rstop.AddBitmap(IDB_Map_dogR5, RGB(255, 255, 255));
		Rstop.AddBitmap(IDB_Map_dogR6, RGB(255, 255, 255));
		Rstop.AddBitmap(IDB_Map_dogR7, RGB(255, 255, 255));
		Rstop.AddBitmap(IDB_Map_dogR8, RGB(255, 255, 255));

		
		Lstop.AddBitmap(IDB_Map_dogL1, RGB(255, 255, 255));
		Lstop.AddBitmap(IDB_Map_dogL2, RGB(255, 255, 255));
		Lstop.AddBitmap(IDB_Map_dogL3, RGB(255, 255, 255));
		Lstop.AddBitmap(IDB_Map_dogL4, RGB(255, 255, 255));
		Lstop.AddBitmap(IDB_Map_dogL5, RGB(255, 255, 255));
		Lstop.AddBitmap(IDB_Map_dogL6, RGB(255, 255, 255));
		Lstop.AddBitmap(IDB_Map_dogL7, RGB(255, 255, 255));
		Lstop.AddBitmap(IDB_Map_dogL8, RGB(255, 255, 255));


		animationR.AddBitmap(IDB_Map_DogwalkR1, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_DogwalkR2, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_DogwalkR3, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_DogwalkR2, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_Map_DogwalkR1, RGB(255, 255, 255));

		animationL.AddBitmap(IDB_Map_DogwalkL1, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_Map_DogwalkL2, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_Map_DogwalkL3, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_Map_DogwalkL2, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_Map_DogwalkL1, RGB(255, 255, 255));


		fightL.AddBitmap(IDB_Map_DogfightL1, RGB(255, 255, 255));
		fightL.AddBitmap(IDB_Map_DogfightL2, RGB(255, 255, 255));
		fightL.AddBitmap(IDB_Map_DogfightL3, RGB(255, 255, 255));
		fightL.AddBitmap(IDB_Map_DogfightL4, RGB(255, 255, 255));
		fightL.AddBitmap(IDB_Map_DogfightL5, RGB(255, 255, 255));
		fightL.AddBitmap(IDB_Map_DogfightL6, RGB(255, 255, 255));


		fightR.AddBitmap(IDB_Map_DogfightR1, RGB(255, 255, 255));
		fightR.AddBitmap(IDB_Map_DogfightR2, RGB(255, 255, 255));
		fightR.AddBitmap(IDB_Map_DogfightR3, RGB(255, 255, 255));
		fightR.AddBitmap(IDB_Map_DogfightR4, RGB(255, 255, 255));
		fightR.AddBitmap(IDB_Map_DogfightR5, RGB(255, 255, 255));
		fightR.AddBitmap(IDB_Map_DogfightR6, RGB(255, 255, 255));



		stuffImg.LoadBitmap(IDB_Map_rabbitL1, 0xffffff);
	}
	void CMapstuff_BlockDog::Initialize(){
		x = 0;
		y = 0;
		is_alive = true;
		MoveTimes = 4;			//移動模式時間
		MoveTimesHit = 0;		//攻擊時動畫時間
		dir = 0;				//方向
		nX = 0;					//變動位置X
		nY = 0;					//變動位置Y
		olddir = 0;				//前次方向
		life = 5;				//生命
		Is_fight = false;
		Hit = false;
	}

	void CMapstuff_BlockDog::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			if (dir == 1 || dir == 4 && Is_fight == false && Is_fight == false){
				animationL.SetTopLeft(x, y);
				animationL.OnShow();
			}
			else if (dir == 2 || dir == 3 && Is_fight == false && Is_fight == false){
				animationR.SetTopLeft(x, y);
				animationR.OnShow();
			}
			else if (Is_fight==true)
			{
				if (olddir == 2 || olddir == 3){
					fightR.SetTopLeft(x, y-50);
					fightR.OnShow();
				}
				else
				{
					fightL.SetTopLeft(x, y-50);
					fightL.OnShow();
				}
			}
			else{
				if (olddir == 2 || olddir == 3){
					Rstop.SetTopLeft(x, y-50);
					Rstop.OnShow();
				}
				else
				{
					Lstop.SetTopLeft(x, y-50);
					Lstop.OnShow();
				}
			}
		}

	}
	void CMapstuff_BlockDog::OnMove(CgameMap *Map){

		int STEP_SIZE = 4;
		int oldnx=nX, oldny=nY;
		animationR.OnMove();
		animationL.OnMove();
		Rstop.OnMove();
		Lstop.OnMove();
		fightL.OnMove();
		fightR.OnMove();
		if (life <= 0)SetIsAlive(false);
		if (MoveTimes == 0 ){
			if (dir <= 4)olddir = dir;
			dir = rand() % 20 + 1;
			MoveTimes = 16;
		}
		else MoveTimes--;
		if (MoveTimesHit > 0){
			MoveTimesHit--;
			//nX += 20;
			STEP_SIZE = -10;
			if (dir == 1 && is_alive )
			{
				nX -= STEP_SIZE;
				nY -= STEP_SIZE;
			}
			if (dir == 2 && is_alive)
			{
				nX += STEP_SIZE;
				nY += STEP_SIZE;
			}
			if (dir == 3 && is_alive)
			{
				nX += STEP_SIZE;
				nY -= STEP_SIZE;
			}
			if (dir == 4 && is_alive)
			{
				nX -= STEP_SIZE;
				nY += STEP_SIZE;
			}
		}
		
		if (dir == 1 && is_alive && Is_fight == false)
		{
			nX -= STEP_SIZE;
			nY -= STEP_SIZE;
		}
		if (dir == 2 && is_alive && Is_fight == false)
		{
			nX += STEP_SIZE;
			nY += STEP_SIZE;
		}
		if (dir == 3 && is_alive && Is_fight == false)
		{
			nX += STEP_SIZE;
			nY -= STEP_SIZE;
		}
		if (dir == 4 && is_alive && Is_fight==false)
		{
			nX -= STEP_SIZE;
			nY += STEP_SIZE;
		}

		x = nX + Map->GetX1() + BlockX*Map->GetMW() / 2;
		y = nY + Map->GetY1() + BlockY*Map->GetMH() / 2;
		if (x<Map->GetX1() + 100 || x>Map->GetX2() - 100)nX = oldnx;
		if (y<Map->GetY1() + 100 || y>Map->GetY2() - 100)nY = oldny;
		x = nX + Map->GetX1() + BlockX*Map->GetMW() / 2;
		y = nY + Map->GetY1() + BlockY*Map->GetMH() / 2;
		if (fightL.IsFinalBitmap() == true && Is_fight)Hit = true;
	}
	void CMapstuff_BlockDog::TraceMove(CgameMap *Map, CRole *Role){
		int oldnx = nX, oldny = nY;
		int STEP_SIZE = 6;
		animationR.OnMove();
		animationL.OnMove();
		Rstop.OnMove();
		Lstop.OnMove();
		fightL.OnMove();
		fightR.OnMove();
		if (life <= 0)SetIsAlive(false);
		if (MoveTimesHit > 0)MoveTimesHit--;
		if (MoveTimesHit > 0){
			MoveTimesHit--;
			//nX += 20;
			STEP_SIZE = -10;
			if (Role->GetX1() > x )	nX += STEP_SIZE;
			if (Role->GetX1() < x )	nX -= STEP_SIZE;
			if (Role->GetY1() < y )nY -= STEP_SIZE;
			if (Role->GetY1() > y )nY += STEP_SIZE;
		}
		/**/
		if (Role->GetX1() > x && Is_fight == false)	nX += STEP_SIZE;
		if (Role->GetX1() < x && Is_fight == false)	nX -= STEP_SIZE;
		if (Role->GetY1() < y && Is_fight == false)nY -= STEP_SIZE;
		if (Role->GetY1() > y && Is_fight == false)nY += STEP_SIZE;

		x = nX + Map->GetX1() + BlockX*Map->GetMW() / 2;
		y = nY + Map->GetY1() + BlockY*Map->GetMH() / 2;
		if (x<Map->GetX1() + 100 || x>Map->GetX2() - 100)nX = oldnx;
		if (y<Map->GetY1() + 100 || y>Map->GetY2() - 100)nY = oldny;
		x = nX + Map->GetX1() + BlockX*Map->GetMW() / 2;
		y = nY + Map->GetY1() + BlockY*Map->GetMH() / 2;
		
		if (Role->GetX1() > x && Role->GetY1() > y && Is_fight == false)dir = 2;
		else if (Role->GetX1() < x && Role->GetY1() < y && Is_fight == false)dir = 1;
		else if (Role->GetX1() > x && Role->GetY1() < y && Is_fight == false)dir = 3;
		else if (Role->GetX1() < x && Role->GetY1() > y && Is_fight == false)dir = 4;
	}
	void CMapstuff_BlockDog::Setfight(bool Is){
		Is_fight = Is;
	}
	bool CMapstuff_BlockDog::GetHit(){
		if (Hit == true){
			Hit = false;
			return true;
		}
		return false;
	}
	int CMapstuff_BlockDog::OnHit(){
		if (MoveTimesHit > 0){
			Hit = false;
			Is_fight = false;
		}
		else if(IsAlive()){
			life--;
			MoveTimesHit = 10;
			if (life == 0)return 1;
		}
		return 0;
	}
	int  CMapstuff_BlockDog::Getlife(){
		return life;
	}
	/////////////////////////////////////////////////////////////////////////////
	//營火Campfire
	/////////////////////////////////////////////////////////////////////////////
	CMapstuff_campfire::CMapstuff_campfire(int InBlockX, int InBlockY) :CMapstuff(InBlockX, InBlockY)
	{
		Initialize();
	}
	void CMapstuff_campfire::LoadBitmap(){
		stuffImg.LoadBitmap(IDB_item_campfire, RGB(255,255,255));
		BCampFire.LoadBitmap(IDB_item_campfire_1, RGB(0, 0, 0));
	}
	void CMapstuff_campfire::Initialize(){
		x = 0;
		y = 0;
		inX = 0;
		inY = 0;
		//life = 5;					//生命值
		is_alive = true;
		time = true;
	}
	void CMapstuff_campfire::OnShow()//地圖位置轉換座標
	{
		if (IsAlive()){
			if (time == true){
				stuffImg.SetTopLeft(x, y);
				stuffImg.ShowBitmap(1.5);
			}
			else if(time==false){
				BCampFire.SetTopLeft(x-35, y-20);
				BCampFire.ShowBitmap(1.5);
			}
		}
	}
	void CMapstuff_campfire::SetCampfire(int nX, int nY, CgameMap *Map){
		inX = nX - Map->GetX1();
		inY = nY - Map->GetY1();
	}
	void CMapstuff_campfire::OnMove(CgameMap *Map){
		x = inX + Map->GetX1();
		y = inY + Map->GetY1()+50;
	}
	void CMapstuff_campfire::SetTime(bool flag){//設定白天晚上
		time = flag;
	}
///////////
//Clock時鐘
///////////////////
	Clock::Clock(){
		time = 0;
		IsRun = true;
		x = SIZE_X - 121;
		y = 10;
		days = 1;
	}
	void Clock::LoadBitmap(){		// 載入圖形
		timebmp[0].LoadBitmap(IDB_day1, RGB(255, 255, 255));
		timebmp[1].LoadBitmap(IDB_day2, RGB(255, 255, 255));
		timebmp[2].LoadBitmap(IDB_day3, RGB(255, 255, 255));
		timebmp[3].LoadBitmap(IDB_day4, RGB(255, 255, 255));
		timebmp[4].LoadBitmap(IDB_day5, RGB(255, 255, 255));
		timebmp[5].LoadBitmap(IDB_day6, RGB(255, 255, 255));
		timebmp[6].LoadBitmap(IDB_day7, RGB(255, 255, 255));
		timebmp[7].LoadBitmap(IDB_day8, RGB(255, 255, 255));
		dayBD.LoadBitmap(IDB_dayDG, RGB(255, 255, 255));
	}
	void Clock::OnMove(){			// 移動
		time++;
		days=time/1600+1;
	}
	void Clock::OnShow(){		// 將圖形貼到畫面
		int sub=200, start=0;
		for (int i = 0; i < 8; i++){
			if (time%1600 <= start + sub * (i+1) && time%1600 >= start + sub * i){
				timebmp[i].SetTopLeft(x, y);
				timebmp[i].ShowBitmap();
			}
		}
		dayBD.SetTopLeft(0, 0);
		dayBD.ShowBitmap(0.7);
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
		CFont f, *fp;
		f.CreatePointFont(320, "標楷體");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 255));
		string temp = "第";
		if (days / 10>0)temp += (48 + days / 10);
		temp += (48 + days % 10);
		temp += "天";
		pDC->TextOut(25, 23, temp.c_str());
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

	}
	void Clock::SetTime(int ntime){			//設定時間
		time = ntime;
	}
	int Clock::GetTime(){			//取得時間
		return time%1600;
	}
	int Clock::GetDays(){
		return days;
	}
///////////
//讀寫檔CSavefile
///////////////////
CSavefile::CSavefile(){
		x = SIZE_X - 111;
		y = SIZE_Y - 112;
	}
void CSavefile::LoadBitmap(){		// 載入圖形
	Rfiles_Img.LoadBitmap(IDB_ERASER1, RGB(255, 255, 255));
	Wfiles_Img.LoadBitmap(IDB_ERASER2, RGB(255, 255, 255));
	}
void CSavefile::OnMove(){			// 移動
	}
void CSavefile::OnShow(){		// 將圖形貼到畫面
}
void CSavefile::Rfiles(string filename,CRoleState *State, CItem *Item, Clock *clock){
	fstream fin;
	char line[100];
	fin.open(filename, ios::in);
	fin.getline(line, sizeof(line), '\n');
	for (int k = 0; k < 19; k++)Item->Initialize();
	State->SetLife((line[1] - 48) * 100 + (line[2] - 48) * 10 + (line[3] - 48));
	State->SetHungyDegree((line[6] - 48) * 100 + (line[7] - 48) * 10 + (line[8] - 48));
	State->Setbrain((line[11] - 48) * 100 + (line[12] - 48) * 10 + (line[13] - 48));
	for (int i = 0; i < 19; i++){
		for (int j = 0; j < 100; j++)line[j] = 0;
		fin.getline(line, sizeof(line), '\n');
		if (line[1] == 44 && line[3]==0)Item->GetItem(line[0] - 48, line[2] - 48);
		else if (line[2] == 44 && line[4] == 0)Item->GetItem((line[0] - 48) * 10 + line[1] - 48, line[3] - 48);
		else if (line[1] == 44 && line[4] == 0)Item->GetItem(line[0] - 48, (line[2]-48)*10 + line[3]- 48);
		else Item->GetItem((line[0] - 48) * 10 + line[1] - 48, (line[3] - 48) * 10 + line[4] - 48);
	}
	fin.getline(line, sizeof(line), '\n');
	if (line[0] - 48 != 0) clock->SetTime((line[0] - 49) * 1000 + (line[1] - 48) * 100 + (line[2] - 48) * 10 + line[3] - 48);
	fin.close();
}
void CSavefile::Wfiles(string filename,CRoleState *State, CItem *Item ,Clock *clock){
	fstream fp;
	fp.open(filename, ios::out);//開啟檔案

	fp << 1000+State->GetLife() << "," << 1000+State->GetHungyDegree() << "," << 1000+State->Getbrain() << endl;//寫入腳色狀態
	for (int i = 0; i < 19;i++)	fp << Item->GetHaveItem(i).GetId() << "," << Item->GetHaveItem(i).Getamount() << endl;//寫入道具資料
	fp << clock->GetTime()+1000 << endl;
	
	for (int i = 0; i < 40; i++){
		for (int j = 0; j < 40; j++){
			int R = rand() % 20;
			if(R<10)fp << 0 << R << ",";
			else fp << R << ",";
		}
		fp << endl;
	}
	fp.close();//關閉檔案
	
	return ;
}
/////////////////////////////////////////////////////////////////////////////
// CPauseMenu暫停菜單
/////////////////////////////////////////////////////////////////////////////
CPauseMenu::CPauseMenu(){
	IsShow = 0;
	x = 100;
	y = 100;
}
void CPauseMenu::LoadBitmap(){		// 載入圖形
	PauseMenuImp.LoadBitmap(IDB_PauseMenuImg, RGB(255, 255, 255));
	PauseButtonImp.LoadBitmap(IDB_PauseButton, RGB(255, 255, 255));
}
void CPauseMenu::OnShow(){			// 將圖形貼到畫面
	PauseButtonImp.SetTopLeft(SIZE_X-PauseButtonImp.Width(), SIZE_Y-PauseButtonImp.Height()*0.5);
	PauseButtonImp.ShowBitmap(0.5);
	if (IsShow == 1){
		PauseMenuImp.SetTopLeft(x, y);
		PauseMenuImp.ShowBitmap();
	}
}
int CPauseMenu::IsButton(CPoint P){
	int WD = 145, HD = 50;
	int ButX = x + 105, ButY = y + 210;
	if (IsShow){
		if (P.x > ButX &&					//繼續
			P.x < ButX + WD &&
			P.y > ButY &&
			P.y < ButY + HD)IsShow = 0;
		else if (							//讀檔(回傳2)
			P.x > ButX + WD + 15 &&
			P.x < ButX + WD * 2 + 15 &&
			P.y > ButY &&
			P.y < ButY + HD)return 2;
		else if (							//設置(回傳3)
			P.x > ButX + WD * 2 + 30 &&
			P.x < ButX + WD * 3 + 30 &&
			P.y > ButY &&
			P.y < ButY + HD)return 3;
		else if (							//存檔退出(回傳4)
			P.x > ButX + WD * 3 + 45 &&
			P.x < ButX + 625 &&
			P.y > ButY &&
			P.y < ButY + HD)return 4;
	}
	return (P.x > SIZE_X - PauseButtonImp.Width() &&
		P.x<SIZE_X - PauseButtonImp.Width()*0.5 &&
		P.y> SIZE_Y - PauseButtonImp.Height()*0.5 &&
		P.y < SIZE_Y - PauseButtonImp.Height()*0.5*0.5);
}
void CPauseMenu::SetShow(int flag){
	IsShow = flag;
}
int CPauseMenu::GetShow(){
	return IsShow;
}
/////////////////////////////////////////////////////////////////////////////
// CPauseMenu暫停菜單
/////////////////////////////////////////////////////////////////////////////
COverMenu::COverMenu(){
	IsShow = 0;
	x = 320;
	y = 180;
}
void COverMenu::LoadBitmap(){		// 載入圖形
	OverMenuImp.LoadBitmap(IDB_OverMenu, RGB(255, 255, 255));
	AllBlockImp.LoadBitmap(IDB_AllBlock, RGB(255, 255, 255));
}
void COverMenu::OnShow(int days){			// 將圖形貼到畫面
	if (IsShow == 1){
		AllBlockImp.SetTopLeft(0, 0);
		AllBlockImp.ShowBitmap();
		OverMenuImp.SetTopLeft(x, y);
		OverMenuImp.ShowBitmap(0.7);
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
		CFont f, *fp;
		f.CreatePointFont(220, "標楷體");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(22, 19, 16));
		pDC->SetTextColor(RGB(255, 255, 255));
		string temp;
		if (days / 10>0)temp += (48 + days / 10);
		temp += (48 + days % 10);
		pDC->TextOut(x + 200, y + 80, temp.c_str());
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}
}
int COverMenu::IsButton(CPoint P){
	int WD = 91, HD = 28;
	int ButX = 147 + x, ButY = 217 + y;
	if (IsShow){
		if (P.x > ButX &&					//主選單
			P.x < ButX + WD &&
			P.y > ButY &&
			P.y < ButY + HD){
			IsShow = 0;
			return 1;
		}
		else if (							//重新
			P.x > ButX + WD + 15 &&
			P.x < ButX + WD * 2 + 15 &&
			P.y > ButY &&
			P.y < ButY + HD)return 2;
	}
	return 0;
}
void COverMenu::SetShow(int flag){
	IsShow = flag;
}
int COverMenu::GetShow(){
	return IsShow;
}
/////////////////////////////////////////////////////////////////////////////
// CmimMap
/////////////////////////////////////////////////////////////////////////////
CmimMap::CmimMap(){
	IsShow = 1;
	x = 300;
	y = 70;
}
void CmimMap::LoadBitmap(){		// 載入圖形
	mimMapImg.LoadBitmap(IDB_min_Map, RGB(255, 255, 255));
	BigMapBack.LoadBitmap(IDB_BigMapBack, RGB(255, 255, 255));
	witer.LoadBitmap(IDB_G_Grass, RGB(255, 255, 255));
	forest15.LoadBitmap(IDB_map_forest15, RGB(255, 255, 255));
	minRole.LoadBitmap(IDB_minMap_Role, RGB(255, 255, 255));
	minFire.LoadBitmap(IDB_minMap_fire, RGB(255, 255, 255));
}
void CmimMap::OnShow(CgameMap *map,CMapstuff_campfire *fire,CRole *Role){			// 將圖形貼到畫面
	int MH = 25, MW = 25;
	if (IsShow == 2){
		BigMapBack.SetTopLeft(0, 0);
		BigMapBack.ShowBitmap();
			for (int i = 0; i < 20; ++i){				//顯示地圖資料
				for (int j = 0; j < 20; ++j){
					if (map->GetMap_G(j,i) == 2){
						witer.SetTopLeft(x + (MW*j) - MW, y + (MH*i) - MH);
						witer.ShowBitmap(0.1);
					}
				}
			}
			for (int i = 0; i < 20; ++i){				//顯示地圖資料
				for (int j = 0; j < 20; ++j){
					if (map->GetMap_G(j, i) == 15){
						forest15.SetTopLeft(x + (MW*j) - MW, y + (MH*i) - MH);
						forest15.ShowBitmap(0.1);
					}
				}
			}
				//顯示道具資料
				if (fire->IsAlive()){
				int Fx = (fire->GetX1()-map->GetX1())/map->GetMW(), Fy = (fire->GetY1()-map->GetY1())/map->GetMH();
				minFire.SetTopLeft(x + (MW*(Fx+1)) - MW, y + (MH*(Fy+1)) - MH);
				minFire.ShowBitmap(0.5);
				}
				//顯示角色
				int Rx = (Role->GetX1() - map->GetX1()) / map->GetMW(), Ry = (Role->GetY1() - map->GetY1()) / map->GetMH();
				minRole.SetTopLeft(x + (MW*(Rx+1)) - MW, y + (MH*(Ry+1)) - MH);
				minRole.ShowBitmap(0.5);

					
	}
	if (IsShow >= 1)
	{
		mimMapImg.SetTopLeft(SIZE_X - mimMapImg.Width() - 50, SIZE_Y - mimMapImg.Height()*0.5);
		mimMapImg.ShowBitmap(0.5);
	}

}
int CmimMap::IsButton(CPoint P){
	return (P.x > SIZE_X - mimMapImg.Width()-50 &&
		P.x<SIZE_X - mimMapImg.Width()*0.5-50 &&
		P.y> SIZE_Y - mimMapImg.Height()*0.5 &&
		P.y < SIZE_Y );
}
void CmimMap::SetShow(int flag){
	IsShow = flag;
}
int CmimMap::GetShow(){
	return IsShow;
}
//////////////////////////////////////////////////////////////////////////////
//CHelp
/////////////////////////////////////////////////////////////////////////////
CHelp::CHelp(){
	x = -540;
	y = 10;
	run = 0;
	movenum1 = 0;
	movenum2 = 0;
	movenum3 = 0;
}
void CHelp::OnMove(){
	if (run != 1 && movenum1 + x > -527 && run != 0)movenum1 = 0;
	else if (movenum2 + x > -527 && run != 2 && run != 0)movenum2 = 0;
	else if (movenum3 + x > -527 && run != 3 && run != 0)movenum3 = 0;
	else if (run == 0 && movenum1 + x > -527)movenum1 -= 30;
	else if (movenum2 + x > -527 && run == 0)movenum2 -= 30;
	else if (movenum3 + x > -527 && run == 0)movenum3 -= 30;
	else if (movenum1 + x < 0 && run == 1)movenum1 += 30;
	else if (movenum2 + x < 0 && run == 2)movenum2 += 30;
	else if (movenum3 + x < 0 && run == 3)movenum3 += 30;
}
void CHelp::OnShow(){

	ControlImg.SetTopLeft(x + movenum1, y);
	GoalImg.SetTopLeft(x + movenum2, y);
	ItemImg.SetTopLeft(x + movenum3, y);
	
	if ( movenum1 >= 5){
		ControlImg.ShowBitmap(0.85);
		GoalImg.ShowBitmap(0.85);
		ItemImg.ShowBitmap(0.85);
	}
	else if (movenum2 >= 5){
		GoalImg.ShowBitmap(0.85);
		ItemImg.ShowBitmap(0.85);
		ControlImg.ShowBitmap(0.85);
	}
	else if ( movenum3 >= 5){
		ItemImg.ShowBitmap(0.85);
		GoalImg.ShowBitmap(0.85);
		ControlImg.ShowBitmap(0.85);
	}
	else {
		ItemImg.ShowBitmap(0.85);
		GoalImg.ShowBitmap(0.85);
		ControlImg.ShowBitmap(0.85);
	}
}
void CHelp::LoadBitmap(){
	ControlImg.LoadBitmap(IDB_explain_left1, RGB(255, 255, 255));
	GoalImg.LoadBitmap(IDB_explain_left2, RGB(255, 255, 255)); 
	ItemImg.LoadBitmap(IDB_explain_left3, RGB(255, 255, 255));
}
void CHelp::SetRun(int flag){
	run = flag;
}
int  CHelp::GetRun(){
	return run;
}
int  CHelp::IsMouse(CPoint *point){
	int oldrun = run;
	int Px = point->x,	Py = point->y;
	if (Py < 187 && Py>46 && Px < x + 595 + movenum1)run = 1;
	else if (Py < 360 && Py>220 && Px < x + 595 + movenum2)run = 2;
	else if (Py < 555 && Py>390 && Px < x + 595 + movenum3)run = 3;
	if (oldrun != run)CAudio::Instance()->Play(16, false);			// 撥放 音樂
	if (Px > x + 595 + movenum3 + movenum2 + movenum1)run = 0;
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CTEXT
/////////////////////////////////////////////////////////////////////////////
CText::CText(){
	x = 0;
	y = SIZE_Y - 187;
	IsShow = false;
	movenum = 187;
	Massage = "";
	Txtscale = 14;
	StoryVariable = 10;//章節
	KillRabbit = 0;
	KillBilockDog = 0;
}

void CText::OnMove(){
	mouse.OnMove();
	if (Massage == "" && movenum < 187)movenum += 20;
	else if (Massage != "" && movenum > 0) movenum -= 20;
	if (movenum > 180)IsShow = false;
	else IsShow = true;
	char a = (48 + StoryVariable/10);
	string temp = ".\\Lins\\0";
	temp += a;
	temp += ".txt";
	Readfile(temp.c_str(), StoryVariable%10+1, 48);
}
void CText::OnShow(){
	TextBackGroungImg.SetTopLeft(x, y + movenum);
	TextBackGroungImg.ShowBitmap();
	CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
	CFont f, *fp;
	f.CreatePointFont(Txtscale*10, "標楷體");	// 產生 font f; 160表示16 point的字
	fp = pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0, 0, 0));
	pDC->SetTextColor(RGB(255, 255, 255));
	string STR = Massage;
	pDC->TextOut(x + 50, y + 50 + movenum, STR.c_str());
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	if (movenum <= 10){
		mouse.SetTopLeft(SIZE_X - 150, SIZE_Y - 100);
		mouse.OnShow();
	}
}
void CText::LoadBitmap(){
	TextBackGroungImg.LoadBitmap(IDB_TEXT, RGB(255, 255, 255));
	mouse.AddBitmap(IDB_mouse1, RGB(255, 255, 255));
	mouse.AddBitmap(IDB_mouse2, RGB(255, 255, 255));
}
void CText::SetIsShow(bool nIsShow){
	IsShow = nIsShow;
}
bool CText::GetIsShow(){
	return IsShow;
}
void CText::SetStoryVariable(int nStoryVariable){
	StoryVariable = nStoryVariable;
}
int CText::GetStoryVariable(){
	return StoryVariable;
}
void CText::SetText(string Text,int nTxtscale){
	Massage = Text;
	Txtscale = nTxtscale;
}
void CText::Readfile(string filename, int RowNum,int nTxtscale){
	fstream fin;
	string temp;
	char line[100];
	fin.open(filename, ios::in);
	for (int i = 0; i < RowNum;i++)fin.getline(line, sizeof(line), '\n');
	temp = &line[0];
	Massage = temp;
	fin.close();
	Txtscale = nTxtscale;
}
void CText::SetKillAnimal(int ID, int num){
	if (ID == 1)KillRabbit+=num;
	else if (ID == 2)KillBilockDog += num;
	return;
}
int CText::GetKillAniml(int ID){
	if (ID == 1)return KillRabbit;
	else if (ID == 2)return  KillBilockDog;
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// 製造列
/////////////////////////////////////////////////////////////////////////////

CMake::CMake()
{
	Initialize();
}
int CMake::GetX1()
{
	return x;
}

int CMake::GetY1()
{
	return y;
}

int CMake::GetX2()
{
	return x + Makeform.Width()*0.5;
}
int CMake::GetY2()
{
	return y + Makeform.Height()*0.5;
}
void CMake::Initialize()
{
	const int X_POS = 0;
	const int Y_POS = 80;
	x = X_POS;
	y = Y_POS;
	hud_tools_Show = 0;
	AxesShow = 0;
	spadeShow = 0;
	pickaxeShow = 0;
}

void CMake::LoadBitmap()
{
	Makeform.LoadBitmap(IDB_MakeForm, RGB(255, 255, 255));
	AxesYesform.LoadBitmap(IDB_hud_axeYes, RGB(255, 255, 255));
	AxesNoform.LoadBitmap(IDB_hud_axeNo, RGB(255, 255, 255));
	campfireYesform.LoadBitmap(IDB_hud_campfireYes,RGB(255, 255, 255));
	campfireNoform.LoadBitmap(IDB_hud_campfireNo,RGB(255, 255, 255));
	torchfireYesform.LoadBitmap(IDB_hud_torchfireYes,RGB(255, 255, 255));
	torchfireNoform.LoadBitmap(IDB_hud_torchfireNo,RGB(255, 255, 255));
	spadeYesform.LoadBitmap(IDB_hud_spadeYes, RGB(255, 255, 255));
	spadeNoform.LoadBitmap(IDB_hud_spadeNo, RGB(255, 255, 255));
	pickaxeYesform.LoadBitmap(IDB_hud_pickaxeYes, RGB(255, 255, 255));
	pickaxeNoform.LoadBitmap(IDB_hud_pickaxeNo, RGB(255, 255, 255));
	hud_tools_fire.LoadBitmap(IDB_hud_tools_fire, RGB(255, 255, 255));
	hud_tools.LoadBitmap(IDB_hud_tools, RGB(255, 255, 255));
}

void CMake::SetXY(int nx, int ny)
{
	x = nx;
	y = ny;
}
int CMake::OnMouseDown(){
	x += 0;
	y += 0;
	return 1;
}
void CMake::OnShow()
{	
	Makeform.SetTopLeft(x, y);
	Makeform.ShowBitmap(0.5);
	if (hud_tools_Show == 1){
		hud_tools.SetTopLeft(x + Makeform.Width()*0.5, y);
		hud_tools.ShowBitmap(1);
	}
	else if (hud_tools_Show==2)
	{
		hud_tools_fire.SetTopLeft(x + Makeform.Width()*0.5, y);
		hud_tools_fire.ShowBitmap();
	}

	if (AxesShow == 1){
		AxesYesform.SetTopLeft(x + Makeform.Width()*0.5+86, y);
		AxesYesform.ShowBitmap(1);
	}
	else if (AxesShow == 2){
		AxesNoform.SetTopLeft(x + Makeform.Width()*0.5+86, y);
		AxesNoform.ShowBitmap(1);
	}
	if (campfireShow == 1){
		campfireYesform.SetTopLeft(x + Makeform.Width()*0.5+86, y);
		campfireYesform.ShowBitmap(1);
	}
	else if (campfireShow == 2){
		campfireNoform.SetTopLeft(x + Makeform.Width()*0.5+86, y);
		campfireNoform.ShowBitmap(1);
	}
	if (torchfireShow == 1){
		torchfireYesform.SetTopLeft(x + Makeform.Width()*0.5+86, y);
		torchfireYesform.ShowBitmap(1);
	}
	else if (torchfireShow == 2){
		torchfireNoform.SetTopLeft(x + Makeform.Width()*0.5+86, y);
		torchfireNoform.ShowBitmap(1);
	}
	if (pickaxeShow == 1){
		pickaxeYesform.SetTopLeft(x + Makeform.Width()*0.5 + 86, y);
		pickaxeYesform.ShowBitmap(1);
	}
	else if (pickaxeShow == 2){
		pickaxeNoform.SetTopLeft(x + Makeform.Width()*0.5 + 86, y);
		pickaxeNoform.ShowBitmap(1);
	}
	if (spadeShow == 1){
		spadeYesform.SetTopLeft(x + Makeform.Width()*0.5 + 86, y);
		spadeYesform.ShowBitmap(1);
	}
	else if (spadeShow == 2){
		spadeNoform.SetTopLeft(x + Makeform.Width()*0.5 + 86, y);
		spadeNoform.ShowBitmap(1);
	}
}
void CMake::SetMake_AxesOnSow(int flog)
{
	AxesShow = flog;
	return;
}
int CMake::GetMake_AxesOnSow()
{
	return AxesShow;
}
void CMake::SetMake_torchfireOnSow(int flog)
{
	torchfireShow = flog;
	return;
}
int CMake::GetMake_torchfireOnSow()
{
	return torchfireShow;
}
void CMake::SetMake_campfireOnSow(int flog)
{
	campfireShow = flog;
	return;
}
int CMake::GetMake_campfireOnSow()
{
	return campfireShow;
}
void CMake::SetMake_pickaxeShow(int flog)
{
	pickaxeShow = flog;
	return;
}
int CMake::GetMake_pickaxeShow()
{
	return pickaxeShow;
}void CMake::SetMake_spadeShow(int flog)
{
	spadeShow = flog;
	return;
}
int CMake::GetMake_spadeShow()
{
	return spadeShow;
}
void CMake::SetMake_hud_tools_Show(int flog)
{
	hud_tools_Show = flog;
	return;
}
int CMake::GetMake_hud_tools_Show()
{
	return hud_tools_Show;
}
bool CMake::BuildHit(int nx,int ny)
{
	int formX1 = 0, formX2 = 0, formY1 = 0,formY2 = 0;
	formX1 = x + Makeform.Width()*0.5 + 120+86;
	formX2 = x + Makeform.Width()*0.5 + AxesYesform.Width()-50+86;
	formY1 = y + AxesYesform.Height()-120;
	formY2 = y + AxesYesform.Height()-65;
	return (formX2 >= nx && formX1 <= nx && formY2 >= ny && formY1 <= ny);
}
bool CMake::MakeformHit(int nx, int ny)
{
	int formX1 = 0, formX2 = 0, formY1 = 0, formY2 = 0;
	formX1 = x + Makeform.Width()*0.5 ;
	formX2 = x + Makeform.Width()*0.5 + AxesYesform.Width()+86 ;
	formY1 = y;
	formY2 = y + AxesYesform.Height();
	return (formX2 >= nx && formX1 <= nx && formY2 >= ny && formY1 <= ny);
}
bool CMake::Enough(CItem item,int ID1,int amount1 ,int ID2,int amount2)
{ 
	
	if (item.LostItem(ID1, amount1) != true){
		item.GetItem(ID1, amount1);
		return 0;
	}
	if (item.LostItem(ID2, amount2) != true){
		item.GetItem(ID2, amount2);
		return 0;
	}
		
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// 道具欄
/////////////////////////////////////////////////////////////////////////////

CItem::CItem()
{
	Initialize();
}

int CItem::GetX1()
{
	return x;
}

int CItem::GetY1()
{
	return y;
}

int CItem::GetX2()
{
	return x + Itemform.Width();
}

int CItem::GetY2()
{
	return y + Itemform.Height();
}

void CItem::Initialize()
{
	const int X_POS = 200;
	const int Y_POS = SIZE_Y;
	x = X_POS;
	y = Y_POS-56 ;
	ItemAmount.SetInteger(0);
	ChoiceItem = 0;
	fireYes = false;
	for (int i = 0; i< 19; i++){
		Have_Item[i].SetId(0);
		Have_Item[i].Setamount(0);
	}
}

void CItem::LoadBitmap()
{
	Itemform.LoadBitmap(IDB_ItemForm, RGB(255, 255, 255));
	Item_flint.LoadBitmap(IDB_flint, RGB(255, 255, 255));
	Item_tree.LoadBitmap(IDB_item_wood, RGB(255, 255, 255));
	Item_Axes.LoadBitmap(IDB_Axes, RGB(255, 255, 255));
	Item_Branch.LoadBitmap(IDB_item_Branch, RGB(255, 255, 255));
	Item_Hay.LoadBitmap(IDB_item_grass, RGB(255, 255, 255));
	Item_stone.LoadBitmap(IDB_item_stone, RGB(255, 255, 255));
	Item_Rabbit.LoadBitmap(IDB_item_Rabbit, RGB(255, 255, 255));
	Item_torchfire.LoadBitmap(IDB_Item_Torchfire, RGB(255, 255, 255));
	Item_RabbitDead.LoadBitmap(IDB_item_RabbitDead, RGB(255, 255, 255));
	face_roast.LoadBitmap(IDB_face_roast, RGB(255, 255, 255));
	face_drop.LoadBitmap(IDB_face_drop, RGB(255, 255, 255));
	Item_spade.LoadBitmap(IDB_item_spade, RGB(255, 255, 255));
	Item_pickaxe.LoadBitmap(IDB_item_pickaxe, RGB(255, 255, 255));
	Item_Carrot.LoadBitmap(IDB_item_Carrot, RGB(255, 255, 255));
	Item_Berry.LoadBitmap(IDB_item_Berry, RGB(255, 255, 255));
	Item_Hat.LoadBitmap(IDB_item_Hat, RGB(255, 255, 255));
	Item_gold.LoadBitmap(IDB_item_gold, RGB(255, 255, 255));
	Item_Honey.LoadBitmap(IDB_item_Honey, RGB(255, 255, 255));
	ItemAmount.LoadBitmap();
}

void CItem::SetXY(int nx, int ny)
{
	x = nx;
	y = ny;
}

void CItem::OnShow()
{
	Itemform.SetTopLeft(x, y);//顯示框架
	Itemform.ShowBitmap(0.5);

	
	for (int i = 0; i < 18; i++){						//顯示燧頭
		if (Have_Item[i].GetId() == 1){

			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_flint.SetTopLeft(x + 16 + i * 36, y + 23);
				Item_flint.ShowBitmap(0.45);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y );
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 2){			//顯示樹木
			
			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_tree.SetTopLeft(x + 16 + i * 36, y+23);
				Item_tree.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 3){			//顯示斧頭

			ItemAmount.SetInteger(-Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_Axes.SetTopLeft(x + 14 + i * 36, y+26);
				Item_Axes.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 18 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 4){			//顯示乾草
			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_Hay.SetTopLeft(x + 16 + i * 36, y + 18);
				Item_Hay.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 5){			//顯示樹枝

			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_Branch.SetTopLeft(x + 16 + i * 36, y + 20);
				Item_Branch.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 6){			//顯示石頭
			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_stone.SetTopLeft(x + 16 + i * 36, y + 23);
				Item_stone.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 7){			//顯示兔子
			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_Rabbit.SetTopLeft(x + 20 + i * 36, y + 18);
				Item_Rabbit.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 8){			//顯示火炬
			ItemAmount.SetInteger(-Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_torchfire.SetTopLeft(x + 16 + i * 36, y + 23);
				Item_torchfire.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 18 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 9){			//顯示烤兔子

			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_RabbitDead.SetTopLeft(x + 16 + i * 36, y + 23);
				Item_RabbitDead.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 10){			//顯示鶴嘴鎬

			ItemAmount.SetInteger(-Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_pickaxe.SetTopLeft(x + 16 + i * 36, y + 23);
				Item_pickaxe.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 18 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 11){			//顯示鏟子

			ItemAmount.SetInteger(-Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_spade.SetTopLeft(x + 16 + i * 36, y + 23);
				Item_spade.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 18 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 12){			//顯示帽子

			ItemAmount.SetInteger(-Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_Hat.SetTopLeft(x + 16 + i * 36, y + 23);
				Item_Hat.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 18 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 13){			//顯示黃金

			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_gold.SetTopLeft(x + 16 + i * 36, y + 23);
				Item_gold.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 14){			//顯示紅羅波

			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_Carrot.SetTopLeft(x + 16 + i * 36, y + 18);
				Item_Carrot.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 15){			//顯示漿果

			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_Berry.SetTopLeft(x + 16 + i * 36, y + 23);
				Item_Berry.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
		else if (Have_Item[i].GetId() == 16){			//顯示蜂蜜

			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (ItemAmount.GetInteger() != 0){
				Item_Honey.SetTopLeft(x + 16 + i * 36, y + 23);
				Item_Honey.ShowBitmap(0.5);
				ItemAmount.SetTopLeft(x + 25 + i * 36, y);
				ItemAmount.ShowBitmap();
			}
		}
	}
	return;
}
bool CItem::ChaseMouse(int mX,int mY)		//鼠標顯示
{
	mX -= 18;
	mY -= 18;
	if (GetHaveItem(18).Getamount()==0)ChoiceItem = 0;
	switch (ChoiceItem){
		case 1:									//顯示燧石
			Item_flint.SetTopLeft(mX, mY);
			Item_flint.ShowBitmap(0.55);
			break;
		case 2:
			Item_tree.SetTopLeft(mX, mY);
			Item_tree.ShowBitmap(0.6);			//顯示樹木
			break;
		case 3:
			Item_Axes.SetTopLeft(mX, mY);				//顯示斧頭
			Item_Axes.ShowBitmap(0.6);
			break;
		case 4:
			Item_Hay.SetTopLeft(mX, mY);				//顯示乾草
			Item_Hay.ShowBitmap(0.6);
			break;
		case 5:
			Item_Branch.SetTopLeft(mX, mY);			//顯示樹枝
			Item_Branch.ShowBitmap(0.6);
			break;
		case 6:
			Item_stone.SetTopLeft(mX, mY);			//顯示石頭
			Item_stone.ShowBitmap(0.6);
			break;
		case 7:
			Item_Rabbit.SetTopLeft(mX, mY);			//顯示兔子
			Item_Rabbit.ShowBitmap(0.6);
			break;
		case 8:
			Item_torchfire.SetTopLeft(mX, mY);			//顯示火炬
			Item_torchfire.ShowBitmap(0.6);
			break;
		case 9:
			Item_RabbitDead.SetTopLeft(mX, mY);			//顯示烤兔子
			Item_RabbitDead.ShowBitmap(0.6);
			break;
		case 10:
			Item_pickaxe.SetTopLeft(mX, mY);			//顯示鶴嘴鎬
			Item_pickaxe.ShowBitmap(0.6);
			break;
		case 11:
			Item_spade.SetTopLeft(mX, mY);			//顯示鏟子
			Item_spade.ShowBitmap(0.6);
			break;
		case 12:
			Item_Hat.SetTopLeft(mX, mY);			//顯示帽子
			Item_Hat.ShowBitmap(0.6);
			break;
		case 13:
			Item_gold.SetTopLeft(mX, mY);			//顯示黃金
			Item_gold.ShowBitmap(0.6);
			break;
		case 14:
			Item_Carrot.SetTopLeft(mX, mY);			//顯示紅羅波
			Item_Carrot.ShowBitmap(0.6);
			break;
		case 15:
			Item_Berry.SetTopLeft(mX, mY);			//顯示漿果
			Item_Berry.ShowBitmap(0.6);
			break;
		case 16:
			Item_Honey.SetTopLeft(mX, mY);			//顯示蜂蜜
			Item_Honey.ShowBitmap(0.6);
			break;
		default:
			break;
	}
	if (fireYes == 1){
		face_roast.SetTopLeft(mX+25, mY-25);
		face_roast.ShowBitmap();
	}
	else if (fireYes == 2){
		face_drop.SetTopLeft(mX + 25, mY - 25);
		face_drop.ShowBitmap();
	}
	return 0;
}
void CItem::SetChoiceItem(int ID){
	int amount = 0, ItemID = 0;
	int Max = 10;
	if (ChoiceItem == 3 || ChoiceItem == 8 || ChoiceItem == 10 || ChoiceItem == 11 || ChoiceItem == 12)Max = 99;
	if (ID == 15 && ChoiceItem != 3 && ChoiceItem != 0 && ChoiceItem != 8 && ChoiceItem != 10 && ChoiceItem != 11) return;
	if (ID == 16) return;
	if (ID == 17 && ChoiceItem != 12 && ChoiceItem != 0) return;
	if (ChoiceItem == Have_Item[ID].GetId() != 0 &&
		ChoiceItem != 0 &&
		Have_Item[18].Getamount() + Have_Item[ID].Getamount() <= Max){
		Have_Item[ID].Setamount(Have_Item[18].Getamount() + Have_Item[ID].Getamount());
		Have_Item[18].SetId(0);
		Have_Item[18].Setamount(0);
		ChoiceItem = 0;
	}
	else if (ChoiceItem > 0 && Have_Item[ID].GetId()==0){
		Have_Item[ID].SetId(Have_Item[18].GetId());
		Have_Item[ID].Setamount(Have_Item[18].Getamount());
		Have_Item[18].SetId(0);
		Have_Item[18].Setamount(0);
		ChoiceItem = 0;
	}
	else if(ChoiceItem==0){
		ChoiceItem = Have_Item[ID].GetId();
		Have_Item[18].SetId(ChoiceItem);
		Have_Item[18].Setamount(Have_Item[ID].Getamount());
		Have_Item[ID].SetId(0);
		Have_Item[ID].Setamount(0);
	}
	else if (ChoiceItem > 0 && Have_Item[ID].GetId() != 0){
		ItemID = Have_Item[ID].GetId();
		amount = Have_Item[ID].Getamount();
		Have_Item[ID].SetId(Have_Item[18].GetId());
		Have_Item[ID].Setamount(Have_Item[18].Getamount());
		ChoiceItem = ItemID;
		Have_Item[18].SetId(ChoiceItem);
		Have_Item[18].Setamount(amount);
	}
}
int CItem::GetChoiceItem(){
	if (Have_Item[18].Getamount() == 0)ChoiceItem = 0;
	return ChoiceItem;
}
void CItem::SetfireYes(int flog){
	fireYes = flog;
}
CItem_Data CItem::GetHaveItem(int ID){
	return Have_Item[ID];
}
bool CItem::GetItem(int ItemId,int amount){
	int i = 0;
	int Max = 10;
	if (ItemId == 3 || ItemId == 8 || ItemId == 10 || ItemId == 11 || ItemId == 12)Max = 99;
	for (i = 0; i < 15; i++){
		if (Have_Item[i].GetId() == ItemId && Have_Item[i].Getamount() + amount <= Max){
			Have_Item[i].Setamount(amount + Have_Item[i].Getamount());//疊上一個相同的道具
			ItemAmount.SetInteger(Have_Item[i].Getamount());
			return 1;
		}
	}
	for (i = 0; i < 15; i++){
		
		if (Have_Item[i].GetId() == 0){
			Have_Item[i].SetId(ItemId);
			Have_Item[i].Setamount(amount + Have_Item[i].Getamount());
			ItemAmount.SetInteger(Have_Item[i].Getamount());
			return 1;
		}
	}
	return 0;
}
bool CItem::LostItem(int ItemId,int amount){
	int i = 0;
	if (amount == 999){
		amount = Have_Item[18].Getamount();
		i = 18;
	}
	if ((ItemId == 11 && Have_Item[15].GetId() == 11) ||
		(ItemId == 12 && Have_Item[15].GetId() == 12) ||
		(ItemId == 8 && Have_Item[15].GetId() == 8) ||
		(ItemId == 10 && Have_Item[15].GetId() == 10) ||
		(ItemId == 3 && Have_Item[15].GetId() == 3)) {
		if (amount == 3)i = 15;
	}
	for (i; i < 19; i++){
		if (Have_Item[i].GetId() == ItemId && Have_Item[i].Getamount()>=amount){
			Have_Item[i].Setamount(Have_Item[i].Getamount()-amount);//疊上一個相同的道具
			ItemAmount.SetInteger(Have_Item[i].Getamount());
			if (Have_Item[i].Getamount() == 0)Have_Item[i].SetId(0);
			return 1;
		}
		else if (Have_Item[i].GetId() == ItemId && Have_Item[i].Getamount() == 0){
			Have_Item[i].SetId(0);
			return 0;
		}
	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// 角色狀態
/////////////////////////////////////////////////////////////////////////////
CRoleState::CRoleState(){
	Initialize();
}
void CRoleState::Initialize()
{
	x = SIZE_X - 80;
	y = 160;					// 生命欄左上角座標
	Life = 100;
	Hungy = 100;
	brain = 100;
	Ishurt = 0;
	InvincibleTime = 0;//無敵時間
}
void CRoleState::LoadBitmap(){
	Lifeform.LoadBitmap(IDB_state_hart, RGB(255, 255, 255));
	LifeBaseMap.LoadBitmap(IDB_BALL, RGB(0, 0, 0));
	HungyDegreeform.LoadBitmap(IDB_state_hunger, RGB(255, 255, 255));
	HungyDegreeBaseMap.LoadBitmap(IDB_BALL, RGB(255, 255, 255));
	Brainform.LoadBitmap(IDB_state_brain, RGB(255, 255, 255));
	MoribundScrn.AddBitmap(IDB_MoribundScrn, RGB(255, 255, 255));
	MoribundScrn.AddBitmap(IDB_MoribundScrn2, RGB(255, 255, 255));
}
void CRoleState::OnShow(){
	//////瀕死
	if (Life < 20){
		MoribundScrn.SetDelayCount(Life+3);
		MoribundScrn.SetTopLeft(0, 0);
		MoribundScrn.OnShow();
	}
	if (InvincibleTime > 10){
		MoribundScrn.SetDelayCount(5);
		MoribundScrn.SetTopLeft(0, 0);
		MoribundScrn.OnShow();
	}
	if (Ishurt-- > 0){
		MoribundScrn.SetDelayCount(10);
		MoribundScrn.SetTopLeft(0, 0);
		MoribundScrn.OnShow();
	}
	//////狀態顯示
	CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
	CBrush *pr, r(RGB(255, 0, 0));				// 畫紅色 progress框
	pr = pDC->SelectObject(&r);
	pDC->Rectangle(SIZE_X - 80, (y+10) + (100-Life) * 60 / 100, SIZE_X - 10, (y + 70));
	pDC->Rectangle(SIZE_X - 80, (y + 80+10)  + (100-Hungy) * 60 / 100, SIZE_X - 10, (y + 150));
	pDC->Rectangle(SIZE_X - 80, (y + 160+10) + (100-brain) * 60 / 100, SIZE_X - 10, (y + 230));
	CDDraw::ReleaseBackCDC();


	Lifeform.SetTopLeft(x, y);
	Lifeform.ShowBitmap(1.1);
	HungyDegreeform.SetTopLeft(x, y+80);
	HungyDegreeform.ShowBitmap(1.1);
	Brainform.SetTopLeft(x, y + 160);
	Brainform.ShowBitmap(1.1);

}
void CRoleState::OnMove(int time , CItem Item,bool fire){
	MoribundScrn.OnMove();
	if (time % 40 == 5)Hungy--;
	if (time % 10 == 5 && time > 1200 && Item.GetHaveItem(15).GetId() != 8 && fire==false)brain--;
	if (time % 100 == 99 && time > 1200 && Item.GetHaveItem(15).GetId() != 8 && fire == false){
		Life -= 20;
		Ishurt = 3;
	}
	if (time % 20 == 6 && Hungy < 1)Life--;
	if (time % 20 == 6 && brain < 1)Life--;
	if (Life < 0)Life = 0;
	if (Hungy < 0)Hungy = 0;
	if (brain < 0)brain = 0;
	if (InvincibleTime > 0)InvincibleTime--;
}
void CRoleState::SetLife(int nlife){
	if (nlife < 0)nlife = 0;
	if (nlife > 100)nlife = 100;
	if (nlife > Life)Life = nlife;
	if (nlife < Life && InvincibleTime == 0){
		Life = nlife;
		InvincibleTime = 20;
	}
}
int CRoleState::GetLife(){
	return Life;
}
void CRoleState::SetHungyDegree(int nHungy){	// 設定飽食度
	if (nHungy < 0)nHungy = 0;
	if (nHungy > 100)nHungy = 100;
	Hungy = nHungy;
}
int CRoleState::GetHungyDegree(){			//取得飽食度欄
	return Hungy;
}
void CRoleState::Setbrain(int nbrain){	// 設定飽食度
	if (nbrain < 0)nbrain = 0;
	if (nbrain > 100)nbrain = 100;
	brain = nbrain;
}
int CRoleState::Getbrain(){			//取得飽食度欄
	return brain;
}
/////////////////////////////////////////////////////////////////////////////
// 道具
/////////////////////////////////////////////////////////////////////////////

CItem_Data::CItem_Data()
{
	Initialize();
}
void CItem_Data::Initialize()
{
	ItemId = 0;
	x = 0;
	y = 0;
	amount = 0;
}
void CItem_Data::SetId(int Id)
{
	ItemId = Id;
}
int CItem_Data::GetId(){
	return ItemId;
}
void CItem_Data::Setamount(int nAmount){
	amount = nAmount;
	return;
}
int CItem_Data::Getamount(){
	return amount;
}

/////////////////////////////////////////////////////////////////////////////
//主角
/////////////////////////////////////////////////////////////////////////////
CRole::CRole()
{
	Initialize();
}

int CRole::GetX1()
{
	return x;
}

int CRole::GetY1()
{
	return y;
}

int CRole::GetX2()
{
	return x + animation.Width();
}

int CRole::GetY2()
{
	return y + animation.Height();
}
int CRole::GetDir()
{
	return dir;
}

void CRole::Initialize()
{
	const int X_POS = SIZE_X/2-50;
	const int Y_POS = SIZE_Y/2-50;
	x = X_POS;
	y = Y_POS;
	dir = 0;
	run = false;
	isSPACEShow = 0;
	isSPACE=isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
	IsAnimation = false;
}

void CRole::LoadBitmap()
{
		animationD.SetDelayCount(5);
		animationL.SetDelayCount(5);
		animationR.SetDelayCount(5);
		animationU.SetDelayCount(5);
		Role_Pick.SetDelayCount(3);
		Role_Axe_Hack.SetDelayCount(5);
		Role_PickAxe_Hack.SetDelayCount(5);
		animation.AddBitmap(IDB_Role0, RGB(255, 255, 255));

		animationR.AddBitmap(IDB_RoleR1, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_RoleR2, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_RoleR3, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_RoleR4, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_RoleR5, RGB(255, 255, 255));
		animationR.AddBitmap(IDB_RoleR6, RGB(255, 255, 255));

		animationL.AddBitmap(IDB_RoleL1, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_RoleL2, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_RoleL3, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_RoleL4, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_RoleL5, RGB(255, 255, 255));
		animationL.AddBitmap(IDB_RoleL6, RGB(255, 255, 255));

		animationD.AddBitmap(IDB_RoleD1, RGB(255, 255, 255));
		animationD.AddBitmap(IDB_RoleD2, RGB(255, 255, 255));
		animationD.AddBitmap(IDB_RoleD3, RGB(255, 255, 255));
		animationD.AddBitmap(IDB_RoleD4, RGB(255, 255, 255));
		animationD.AddBitmap(IDB_RoleD5, RGB(255, 255, 255));
		animationD.AddBitmap(IDB_RoleD6, RGB(255, 255, 255));


		animationU.AddBitmap(IDB_RoleU1, RGB(255, 255, 255));
		animationU.AddBitmap(IDB_RoleU2, RGB(255, 255, 255));
		animationU.AddBitmap(IDB_RoleU3, RGB(255, 255, 255));
		animationU.AddBitmap(IDB_RoleU4, RGB(255, 255, 255));
		animationU.AddBitmap(IDB_RoleU5, RGB(255, 255, 255));
		animationU.AddBitmap(IDB_RoleU6, RGB(255, 255, 255));
		animationU.AddBitmap(IDB_RoleU7, RGB(255, 255, 255));
		animationU.AddBitmap(IDB_RoleU8, RGB(255, 255, 255));
		animationU.AddBitmap(IDB_RoleU9, RGB(255, 255, 255));

		Role_Axe.LoadBitmap(IDB_Role_Axe, RGB(255, 255, 255));
		Role_PickAxe.LoadBitmap(IDB_Role_PickAxe2, RGB(255, 255, 255));
		Role_torchfire.LoadBitmap(IDB_Role_torchfire, RGB(255, 255, 255));

		Role_Axe_Hack.AddBitmap(IDB_Role_Axe_Hack1, RGB(255, 255, 255));
		Role_Axe_Hack.AddBitmap(IDB_Role_Axe_Hack2, RGB(255, 255, 255));
		Role_Axe_Hack.AddBitmap(IDB_Role_Axe_Hack3, RGB(255, 255, 255));
		Role_Axe_Hack.AddBitmap(IDB_Role_Axe_Hack2, RGB(255, 255, 255));
		Role_Axe_Hack.AddBitmap(IDB_Role_Axe_Hack1, RGB(255, 255, 255));
		Role_Axe_Hack.AddBitmap(IDB_Role_Axe, RGB(255, 255, 255));

		Role_PickAxe_Hack.AddBitmap(IDB_Role_PickAxe1, RGB(255, 255, 255));
		Role_PickAxe_Hack.AddBitmap(IDB_Role_PickAxe2, RGB(255, 255, 255));
		Role_PickAxe_Hack.AddBitmap(IDB_Role_PickAxe3, RGB(255, 255, 255));
		Role_PickAxe_Hack.AddBitmap(IDB_Role_PickAxe4, RGB(255, 255, 255));
		Role_PickAxe_Hack.AddBitmap(IDB_Role_PickAxe3, RGB(255, 255, 255));
		Role_PickAxe_Hack.AddBitmap(IDB_Role_PickAxe2, RGB(255, 255, 255));
		Role_PickAxe_Hack.AddBitmap(IDB_Role_PickAxe1, RGB(255, 255, 255));
		Role_PickAxe_Hack.AddBitmap(IDB_Role_PickAxe2, RGB(255, 255, 255));

		Role_Pick.AddBitmap(IDB_role_pick1, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick2, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick3, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick1, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick2, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick3, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick1, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick2, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick3, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick1, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick2, RGB(255, 255, 255));
		Role_Pick.AddBitmap(IDB_role_pick3, RGB(255, 255, 255));
		
		Role_Hat.AddBitmap(IDB_Role_Hat1, RGB(255, 255, 255));
		Role_Hat.AddBitmap(IDB_Role_Hat2, RGB(255, 255, 255));

		Role_spade.LoadBitmap(IDB_Role_spade1, RGB(255, 255, 255));
		
		Role_spade_Hack.AddBitmap(IDB_Role_spade1, RGB(255, 255, 255));
		Role_spade_Hack.AddBitmap(IDB_Role_spade2, RGB(255, 255, 255));
		Role_spade_Hack.AddBitmap(IDB_Role_spade2, RGB(255, 255, 255));

}

void CRole::OnMove()//CgameMap* Map)
{
	const int STEP_SIZE = 8;
	animation.OnMove();
	animationR.OnMove();
	animationL.OnMove();
	animationU.OnMove();
	animationD.OnMove();
	Role_Pick.OnMove();
	Role_PickAxe_Hack.OnMove();
	Role_spade_Hack.OnMove();
	Role_Axe_Hack.OnMove();
	if (dir == 1 || dir == 2 || dir == 3 || dir == 4 && IsAnimation == false)Role_Hat.OnMove();
	//x = Map->GetX1() + BlockX*Map->GetMW();					//地圖位置座標
	//y = Map->GetY1() + BlockY*Map->GetMH();					//地圖位置座標

	if (isMovingLeft && run==true)
		x -= STEP_SIZE;
	if (isMovingRight && run == true)
		x += STEP_SIZE;
	if (isMovingUp && run == true)
		y -= STEP_SIZE;
	if (isMovingDown && run == true)
		y += STEP_SIZE;
}

void CRole::SetMovingDown(bool flag)
{
	isMovingDown = flag;
}

void CRole::SetMovingLeft(bool flag)
{
	isMovingLeft = flag;
}

void CRole::SetMovingRight(bool flag)
{
	isMovingRight = flag;
}

void CRole::SetMovingUp(bool flag)
{
	isMovingUp = flag;
}
void CRole::SetSPACE(bool flag)
{
	if (flag == true && isSPACEShow == 0 && (Equipment[0].GetId() == 3 || Equipment[0].GetId() == 10 || Equipment[0].GetId() == 11))isSPACEShow = 15;
	isSPACE = flag;
}
void CRole::SetAnimation(bool flag){
	IsAnimation = flag;
}
bool CRole::GetMovingDown()
{
	return isMovingDown;
}

bool CRole::GetMovingLeft()
{
	return isMovingLeft;
}

bool CRole::GetMovingRight()
{
	return isMovingRight;
}

bool CRole::GetMovingUp()
{
	return isMovingUp;
}
bool CRole::GetSPACE()
{
	return isSPACE;
}
bool CRole::GetAnimation(){			//取得是否結束動畫
	return IsAnimation;
}
void CRole::SetXY(int nx, int ny)
{
	x = nx;
	y = ny;
}

void CRole::OnShow()
{
	if (Role_Pick.IsFinalBitmap()){
		IsAnimation = false;
	}
	if (IsAnimation == true){
		Role_Pick.SetTopLeft(x - 25, y + 15);
		Role_Pick.OnShow();
	}
	else if (dir == 2)
	{
		animationR.SetTopLeft(x, y);
		animationR.OnShow();
	}
	else if (dir == 1)
	{
		animationL.SetTopLeft(x, y);
		animationL.OnShow();
	}
	else if (dir == 3)
	{
		animationU.SetTopLeft(x, y);
		animationU.OnShow();
	}
	else if (dir == 4)
	{
		animationD.SetTopLeft(x, y);
		animationD.OnShow();
	}
	else
	{
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}


	/////顯示裝備
	if (Equipment[0].GetId() == 3 && IsAnimation==false){
		if (isSPACEShow == 0)
		{
			Role_Axe.SetTopLeft(x + animation.Width() - 20, y + animation.Height() / 2);
			Role_Axe.ShowBitmap();
		}
		else if (isSPACEShow>0)
		{
			Role_Axe_Hack.SetTopLeft(x + animation.Width() - 20, y + animation.Height() / 2);
			Role_Axe_Hack.OnShow();
			if (Role_Axe_Hack.IsFinalBitmap() == true)isSPACEShow = 0;
			
		}
	}
	if (Equipment[0].GetId() == 10 && IsAnimation == false){
		if (isSPACEShow == 0)
		{
			Role_PickAxe.SetTopLeft(x + animation.Width() - 45, y + 35);
			Role_PickAxe.ShowBitmap();
		}
		else if (isSPACEShow>0)
		{
			Role_PickAxe_Hack.SetTopLeft(x + animation.Width() - 45, y + 35);
			Role_PickAxe_Hack.OnShow();
			if (Role_PickAxe_Hack.IsFinalBitmap() == true)isSPACEShow = 0;

		}
	}
	if (Equipment[0].GetId() == 11 && IsAnimation == false){
		if (isSPACEShow == 0)
		{
			Role_spade.SetTopLeft(x + animation.Width() - 35, y + 40);
			Role_spade.ShowBitmap();
		}
		else if (isSPACEShow>0)
		{
			Role_spade_Hack.SetTopLeft(x + animation.Width() - 35, y + 40);
			Role_spade_Hack.OnShow();
			if (Role_spade_Hack.IsFinalBitmap() == true)isSPACEShow = 0;
		}
	}
	if (Equipment[0].GetId() == 8){
		Role_torchfire.SetTopLeft(x + animation.Width()-10, y );
		Role_torchfire.ShowBitmap();
	}
	if (Equipment[2].GetId() == 12){
		Role_Hat.SetTopLeft(x-25, y-25);
		Role_Hat.OnShow();
	}
}
void CRole::SetDir(int nDir)
{
	dir = nDir;
}
void CRole::SetRun(bool Nrun){
	run = Nrun;
}
bool CRole::GetRun(){
	return run;
}
bool CRole::HitOther(CMapstuff* stuff,int up){
	int Ry1 = GetY1();				// 人物的左上角x座標
	int Ry2 = GetY2();				// 人物的左上角y座標
	int Rx1 = GetX1();				// 人物的右下角x座標
	int Rx2 = GetX2();				// 人物的右下角y座標
	
	int Tx1 = stuff->GetX1();				// 物件的左上角x座標
	int Tx2 = stuff->GetX2();				// 物件的左上角y座標
	int Ty1 = stuff->GetY1();				// 物件的右下角x座標
	int Ty2 = stuff->GetY2();				// 物件的右下角y座標
	int Hight = Ty2 - Ty1,Width=Tx2-Tx1;
	Tx1 +=Width / 3;
	Ty1 +=Hight / 10 * 9;
	Tx2 -=Width / 3;
	Ty2 +=0;

	Ry1 = Ry2 - 50;
	Rx1 += 20;
	Rx2 -= 20;
	/////擴大判斷
	Rx1 -= up;
	Rx2 += up;
	Ry1 -= up;
	Ry2 += up;

	return (Rx2 >= Tx1 && Rx1 <= Tx2 && Ry2 >= Ty1 && Ry1 <= Ty2);
}
void CRole::RoleItem(CItem *Item)
{
	Equipment[0] = Item->GetHaveItem(15);
	Equipment[1] = Item->GetHaveItem(16);
	Equipment[2] = Item->GetHaveItem(17);
}
bool CRole::HitRectangle(int tx1, int ty1, int tx2, int ty2)
{
	int x1 = x ;				// 人物的左上角x座標
	int y1 = y ;				// 人物的左上角y座標
	int x2 = x1 + animation.Width();	// 人物的右下角x座標
	int y2 = y1 + animation.Height();	// 人物的右下角y座標
	//
	// 檢測球的矩形與參數矩形是否有交集
	//
	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g)
{

}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0);	// 一開始的loading進度為0%
	//
	// 開始載入資料

	logo.LoadBitmap(IDB_GameName, 0xffffff);

	BackGround.LoadBitmap(IDB_StareBackGround);
	CAudio::Instance()->Load(AUDIO_keyin, "sounds\\01_keyin.mp3");				// 載入編號1的聲音
	CAudio::Instance()->Load(AUDIO_keydown, "sounds\\02_keydown.mp3");			// 載入編號2的聲音
	CAudio::Instance()->Load(AUDIO_relax, "sounds\\relax.mp3");	// 載入編號2的聲音relx.mid
	CAudio::Instance()->Load(AUDIO_Slide, "sounds\\16_slide.mp3");
	CAudio::Instance()->Play(AUDIO_relax, true);			// 撥放 背景音樂
	Help.LoadBitmap();
	Sleep(0);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
	for (int i = 10; i <= 80; i++){
		ShowInitProgress(i);	// 接個前一個狀態的進度，此處進度視為i%
	}
}

void CGameStateInit::OnBeginState()
{
	CAudio::Instance()->Resume();			// 撥放 背景音樂
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';
	if (nChar == KEY_SPACE);
       // GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
    else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x < 1140 * 0.8445 && point.x>1018 * 0.8445 && point.y > 475 * 0.8445 && point.y < 520 * 0.8445)
	{
		CAudio::Instance()->Pause();			// 停止 背景音樂
		CAudio::Instance()->Play(AUDIO_keydown, false);
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}
	else if (point.x < 1140 * 0.8445 && point.x>1018 * 0.8445 && point.y > 600 * 0.8445 && point.y < 645 * 0.8445)
	{
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}
}
void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point){
	Help.IsMouse(&point);
}
void CGameStateInit::OnMove(){
	Help.OnMove();
}
void CGameStateInit::OnShow()
{
    // 貼上logo
	BackGround.SetTopLeft(0, 0);
	BackGround.ShowBitmap(0.8445);
	Help.OnShow();
}
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g)
{
}

void CGameStateOver::OnMove()
{
    counter--;
    if (counter < 0)
        GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
    counter = 30 * 3; // 5 seconds
	
	CAudio::Instance()->Stop(AUDIO_relax);			// 停止 背景音樂
}

void CGameStateOver::OnInit()
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
    //
    // 開始載入資料
    //
	DeadMenu.LoadBitmap(IDB_DeadMenu, RGB(255, 255, 255));
    Sleep(0);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 最終進度為100%
    //
    ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    char str[80];								// Demo 數字對字串的轉換
    sprintf(str, "Game Over ! (%d)", counter / 30);
    pDC->TextOut(240, 210, str);
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	DeadMenu.SetTopLeft(0, 0);
	//DeadMenu.SetTopLeft((SIZE_X-DeadMenu.Width())/2,(SIZE_Y-DeadMenu.Height())/2);
	DeadMenu.ShowBitmap(0.8445);
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g)
{
	picX = picY = 0;

}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
	int countflint = 0, counttree = 0,
		countHay = 0, countBranch = 0,
		countstone = 0, countRabbit = 0,
		countCampfire = 0, countBlockDog = 0,
		countHat=0,countgold=0,
		countCarrot=0,countBerry=0;
	ShowInitProgress(0);
	

	Fist_Role.Initialize();
	Item.Initialize();
	RoleState.Initialize();
	GameClock.SetTime(0);
	gamemap.Initialize();
	ShowInitProgress(5);
	for (int i = 0; i < 40; i++){
		for (int j = 0; j < 40; j++){
			switch (gamemap.Loadstuff(i, j)){
			case 0:
				break;
			case 1:
				if (counttree < 90){
				Tree[counttree].SetBlockXY(i, j);
				counttree++;
				}
				break;
			case 2:
				if (countflint<90){
					flint[countflint].SetBlockXY(i, j);
					countflint++;
				}
				break;
			case 4:
				if (countHay<90){
					Hay[countHay].SetBlockXY(i, j);
					countHay++;
				}
				break;
			case 5:
				if (countBranch<90){
					Branch[countBranch].SetBlockXY(i, j);
					countBranch++;
				}
				break;
			case 6:
				if (countstone<90){
					stone[countstone].SetBlockXY(i, j);
					countstone++;
				}
				break;
			case 7:
				if (countRabbit<90){
					Rabbit[countRabbit].SetBlockXY(i, j);
					countRabbit++;
				}
				break;
			case 12:
				if (countHat<90){
					Hat[countHat].SetBlockXY(i, j);
					countHat++;
				}
				break;
			case 13:
				if (countgold<90){
					Gold[countgold].SetBlockXY(i, j);
					countgold++;
				}
				break;
			case 14:
				if (countCarrot<90){
					Carrot[countCarrot].SetBlockXY(i, j);
					countCarrot++;
				}
				break;
			case 15:
				if (countBerry<90){
					Berry[countBerry].SetBlockXY(i, j);
					countBerry++;
				}
				break;
			case 21:
				if (countCampfire<90){
					Campfire[countCampfire].SetBlockXY(i, j);
					countCampfire++;
				}
				break;
			case 16:
				if (countBlockDog<90){
					BlockDog[countBlockDog].SetBlockXY(i, j);
					countBlockDog++;
				}
				break;
			}
		}
	}
	ShowInitProgress(10);
	for (int i = 0; i < 90; i++)Rabbit[i].Initialize();
	for (int i = 0; i < 90; i++)BlockDog[i].Initialize();
	for (int i = 0; i < 10; i++)Campfire[i].Initialize();

	for (int j = countflint; j < 90; j++)flint[j].SetBlockXY(-10, -10);
	for (int i = counttree; i < 90; i++)Tree[i].SetBlockXY(-10, -10);
	for (int i = countHay; i < 90; i++)Hay[i].SetBlockXY(-10, -10);
	for (int i = countBranch; i < 90; i++)Branch[i].SetBlockXY(-10, -10);
	for (int i = countstone; i < 90; i++)stone[i].SetBlockXY(-10, -10);
	for (int i = countRabbit; i < 90; i++)Rabbit[i].SetBlockXY(-10, -10);
	
	for (int i = 1; i < 90; i++)Hat[i].SetBlockXY(-10, -10);
	for (int i = 20; i < 90; i++)Gold[i].SetBlockXY(-10, -10);
	for (int i = countCarrot; i < 90; i++)Carrot[i].SetBlockXY(-10, -10);
	for (int i = countBerry; i < 90; i++)Berry[i].SetBlockXY(-10, -10);
	

	for (int i = 20; i < 90; i++)BlockDog[i].SetBlockXY(-10, -10);
	for (int i = countCampfire; i < 10; i++)Campfire[i].SetCampfire(-5000, -5000, &gamemap);

	CAudio::Instance()->Play(AUDIO_SunBGM, false);
	Savefile.Rfiles(".\\Save\\Init.txt", &RoleState, &Item, &GameClock);
	for (int t = 50; t < 500; t++){
		ShowInitProgress(t/5);
	}
	
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
		Massage.OnMove();
	if (PauseMenu.GetShow() == 0 && Massage.GetIsShow() == false && GameOver.GetShow() == 0){						//開啟暫停
			Sea.OnMove(&gamemap);
			Fist_Role.RoleItem(&Item);//同步裝備欄
			GameClock.OnMove();
		if (gamemap.GetRun() == true && Fist_Role.GetAnimation()==false)gamemap.OnMove(&Fist_Role, &Tree[0], &stone[0]);		//碰撞樹

		if (Make.GetMake_AxesOnSow() >= 1){										//顯示是否可以建造斧頭
			if (Make.Enough(Item, 1, 1, 5, 1))Make.SetMake_AxesOnSow(1);
			else Make.SetMake_AxesOnSow(2);
		}
		if (Make.GetMake_campfireOnSow() > 0){
			if (Make.Enough(Item, 2, 2, 4, 3))Make.SetMake_campfireOnSow(1);
			else Make.SetMake_campfireOnSow(2);
		}
		if (Make.GetMake_torchfireOnSow() > 0){
			if (Make.Enough(Item, 4, 2, 5, 2))Make.SetMake_torchfireOnSow(1);
			else Make.SetMake_torchfireOnSow(2);
		}
		if (Make.GetMake_pickaxeShow() > 0){
			if (Make.Enough(Item, 1, 2, 5, 2))Make.SetMake_pickaxeShow(1);
			else Make.SetMake_pickaxeShow(2);
		}
		if (Make.GetMake_spadeShow() > 0){
			if (Make.Enough(Item, 1, 2, 5, 2))Make.SetMake_spadeShow(1);
			else Make.SetMake_spadeShow(2);
		}
		//
		Fist_Role.OnMove();
		for (int i = 0; i < 90; i++){
			Tree[i].OnMove(&gamemap);
		}
		for (int j = 0; j < 90; j++){
			flint[j].OnMove(&gamemap);
		}
		for (int k = 0; k < 90; k++){
			Branch[k].OnMove(&gamemap);
		}
		for (int w = 0; w < 90; w++){
			Hay[w].OnMove(&gamemap);
		}
		for (int c = 0; c < 90; c++){
			stone[c].OnMove(&gamemap);
		}
		for (int v = 0; v < 90; v++){
			Hat[v].OnMove(&gamemap);
		}
		for (int e = 0; e < 90; e++){
			Gold[e].OnMove(&gamemap);
		}
		for (int c = 0; c < 90; c++){
			Carrot[c].OnMove(&gamemap);
		}
		for (int c = 0; c < 90; c++){
			Berry[c].OnMove(&gamemap);
		}
		for (int v = 0; v < 90; v++){
			if (Fist_Role.HitOther(&Rabbit[v], 100)){
				Rabbit[v].avoidMove(&gamemap, &Fist_Role);
			}
			else Rabbit[v].OnMove(&gamemap);
		}
		for (int v = 0; v < 90; v++){
			if (Fist_Role.HitOther(&BlockDog[v], 50)){
				BlockDog[v].Setfight(true);
				BlockDog[v].OnMove(&gamemap);
			}
			else if (Fist_Role.HitOther(&BlockDog[v], 200)){
				BlockDog[v].TraceMove(&gamemap, &Fist_Role);
			    BlockDog[v].Setfight(false);
			}
			else {
				BlockDog[v].Setfight(false);
				BlockDog[v].OnMove(&gamemap);
			}
			if (BlockDog[v].IsAlive() && BlockDog[v].GetHit()){
				RoleState.SetLife(RoleState.GetLife() - 10);
				CAudio::Instance()->Play(14, false);
			}
		}
		for (int v = 0; v < 10; v++){
			Campfire[v].OnMove(&gamemap);
		}
		///////////角色動作
		for (int i = 0; i < 90; i++)
		{
			if (Fist_Role.GetSPACE() == true && Tree[i].IsAlive() && Fist_Role.HitOther(&Tree[i], 30) && (Item.GetHaveItem(15).GetId() == 3)){
				Tree[i].SetIsAlive(false);
				Item.GetItem(2, 1);
				Item.LostItem(3, 3);
				CAudio::Instance()->Play(11, false);
				CAudio::Instance()->Play(9, false);
				Fist_Role.SetSPACE(false);
			}
			if (Fist_Role.GetSPACE() == true && flint[i].IsAlive() && Fist_Role.HitOther(&flint[i], 30)){
				flint[i].SetIsAlive(false);
				Item.GetItem(1, 1);
				CAudio::Instance()->Play(8, false);
				CAudio::Instance()->Play(9, false);
				Fist_Role.SetSPACE(false);
				Fist_Role.SetAnimation(true);
			}
			if (Fist_Role.GetSPACE() == true && Hay[i].IsAlive() && Fist_Role.HitOther(&Hay[i], 30)){
				Hay[i].SetIsAlive(false);
				Item.GetItem(4, 1);
				CAudio::Instance()->Play(8, false);
				CAudio::Instance()->Play(9, false);
				Fist_Role.SetSPACE(false);
				Fist_Role.SetAnimation(true);
			}
			if (Fist_Role.GetSPACE() == true && Branch[i].IsAlive() && Fist_Role.HitOther(&Branch[i], 30)){
				Branch[i].SetIsAlive(false);
				Item.GetItem(5, 1);
				CAudio::Instance()->Play(8, false);
				CAudio::Instance()->Play(9, false);
				Fist_Role.SetSPACE(false);
				Fist_Role.SetAnimation(true);

			}
			if (Fist_Role.GetSPACE() == true && Hat[i].IsAlive() && Fist_Role.HitOther(&Hat[i], 30)){
				Hat[i].SetIsAlive(false);
				Item.GetItem(12, 1);
				CAudio::Instance()->Play(9, false);
				Fist_Role.SetSPACE(false);
			}
			if (Fist_Role.GetSPACE() == true && Gold[i].IsAlive() && Fist_Role.HitOther(&Gold[i], 30)){
				Gold[i].SetIsAlive(false);
				Item.GetItem(13, 1);
				CAudio::Instance()->Play(8, false);
				CAudio::Instance()->Play(9, false);
				Fist_Role.SetSPACE(false);
				Fist_Role.SetAnimation(true);
			}
			if (Fist_Role.GetSPACE() == true && Carrot[i].IsAlive() && Fist_Role.HitOther(&Carrot[i], 30)){
				Carrot[i].SetIsAlive(false);
				Item.GetItem(14, 1);
				CAudio::Instance()->Play(8, false);
				CAudio::Instance()->Play(9, false);
				Fist_Role.SetSPACE(false);
				Fist_Role.SetAnimation(true);
			}
			if (Fist_Role.GetSPACE() == true && Berry[i].IsAlive() && Fist_Role.HitOther(&Berry[i], 30)){
				Berry[i].SetIsAlive(false);
				Item.GetItem(15, 1);
				CAudio::Instance()->Play(8, false);
				CAudio::Instance()->Play(9, false);
				Fist_Role.SetSPACE(false);
				Fist_Role.SetAnimation(true);
			}
			if (Fist_Role.GetSPACE() == true && stone[i].IsAlive() && Fist_Role.HitOther(&stone[i], 30) && (Item.GetHaveItem(15).GetId() == 10)){
				stone[i].SetIsAlive(false);
				Item.GetItem(6, 1);
				Item.LostItem(10, 3);
				CAudio::Instance()->Play(10, false);
				CAudio::Instance()->Play(9, false);
				Fist_Role.SetSPACE(false);
			}
			if (Fist_Role.GetSPACE() == true && Rabbit[i].IsAlive() && Fist_Role.HitOther(&Rabbit[i], 30) && (Item.GetHaveItem(15).GetId() == 3)){
				Rabbit[i].SetIsAlive(false);
				Item.GetItem(7, 1);
				Item.LostItem(3, 3);
				CAudio::Instance()->Play(9, false);
				Fist_Role.SetSPACE(false);
			}
			if (Fist_Role.GetSPACE() == true && BlockDog[i].IsAlive() && Fist_Role.HitOther(&BlockDog[i], 80) && (Item.GetHaveItem(15).GetId() == 3 || Item.GetHaveItem(15).GetId() == 10 || Item.GetHaveItem(15).GetId() == 11)){
				Massage.SetKillAnimal(2, BlockDog[i].OnHit());
				if (BlockDog[i].Getlife()<=0){
					Item.GetItem(9, 2);
					CAudio::Instance()->Play(9, false);
				}
				Item.LostItem(Item.GetHaveItem(15).GetId(), 3);
				CAudio::Instance()->Play(13, false);
				Fist_Role.SetSPACE(false);
			}
		}
		RoleState.OnMove(GameClock.GetTime(), Item,Fist_Role.HitOther(&Campfire[0], 160));//狀態控制
		
		//////////
		if (RoleState.GetLife() == 0){
			GameOver.SetShow(1);
			CAudio::Instance()->Play(12, false);
			//死亡
		}
	}
	if (GameClock.GetDays() > 2 && Massage.GetStoryVariable() < 20){
		Massage.SetStoryVariable(20);//兩天
		Massage.SetKillAnimal(2, 0);
	}
	else if (Massage.GetKillAniml(2) >= 5 && GameClock.GetDays() > 2 && Massage.GetStoryVariable() < 30){
		Massage.SetStoryVariable(30);//兩天&&殺五隻狗
	}
		
	else if (Massage.GetStoryVariable() < 40 && Massage.GetStoryVariable() > 32){
		if (Item.LostItem(13, 10)){
			Massage.SetStoryVariable(40);
		}
	}
	else if (Massage.GetStoryVariable() > 43){
			RoleState.SetLife(0);
	}
	if (GameClock.GetTime() % 100 ==99 && Item.GetHaveItem(15).GetId() == 8)Item.LostItem(8, 3);
	Fist_Role.SetSPACE(false);
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
	int countflint = 0, counttree = 0,
		countHay = 0, countBranch = 0,
		countstone = 0, countRabbit = 0,
		countCampfire = 0, countBlockDog=0,
		countHat = 0, countgold = 0,
		countCarrot = 0, countBerry = 0;
	//for (int i = 0; i <= 50; i++){
	//	ShowInitProgress(i);	// 接個前一個狀態的進度，此處進度視為33%
	//}
	//
    // 開始載入資料
    //
	GameClock.SetTime(0);
	Fist_Role.LoadBitmap();
	Sea.LoadBitmap();
    //
    // 完成部分Loading動作，提高進度
    //
   // ShowInitProgress(50);
   // Sleep(10); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 繼續載入其他資料
    //

	black100.LoadBitmap(IDB_Black);
	blackO.LoadBitmap(IDB_blackO, 0xffffff);
	GameClock.LoadBitmap();
	
	for (int i = 0; i < 90; i++)Tree[i].LoadBitmap();
	for (int j = 0; j < 90; j++) flint[j].LoadBitmap();
	for (int j = 0; j < 90; j++) Hay[j].LoadBitmap();		
	for (int j = 0; j < 90; j++) Branch[j].LoadBitmap();
	for (int j = 0; j < 90; j++) stone[j].LoadBitmap();
	for (int j = 0; j < 90; j++) Rabbit[j].LoadBitmap();
	for (int j = 0; j < 90; j++) BlockDog[j].LoadBitmap();
	for (int i = 0; i < 10; i++) Campfire[i].LoadBitmap();
	for (int j = 0; j < 90; j++) Hat[j].LoadBitmap();
	for (int j = 0; j < 90; j++) Gold[j].LoadBitmap();
	for (int j = 0; j < 90; j++) Carrot[j].LoadBitmap();
	for (int i = 0; i < 90; i++) Berry[i].LoadBitmap();
	gamemap.LoadBitmap();
	
	Make.LoadBitmap();
	Item.LoadBitmap();
	RoleState.LoadBitmap();
	PauseMenu.LoadBitmap();
	GameOver.LoadBitmap();
	minMap.LoadBitmap();
	text_willdark.LoadBitmap(IDB_text_willdark, 0xffffff);
	text_willsum.LoadBitmap(IDB_text_willsun, 0xffffff);
	Massage.LoadBitmap();
	CAudio::Instance()->Load(AUDIO_walk, "sounds\\03_walk.mp3");				// 載入編號3的聲音
	CAudio::Instance()->Load(AUDIO_NightBGM, "sounds\\04_NightBGM.mp3");			// 載入編號4的聲音
	CAudio::Instance()->Load(AUDIO_NightToSunBGM, "sounds\\05_NightToSunBGM.mp3");		// 載入編號5的聲音
	CAudio::Instance()->Load(AUDIO_SunBGM, "sounds\\06_SunBGM.mp3");				// 載入編號6的聲音
	CAudio::Instance()->Load(AUDIO_SunToNightBGM, "sounds\\07_SunToNightBGM.mp3");		// 載入編號7的聲音
	CAudio::Instance()->Load(AUDIO_pick, "sounds\\08_pick.mp3");		// 載入編號8的聲音
	CAudio::Instance()->Load(AUDIO_Get, "sounds\\09_getitem.mp3");		// 載入編號9的聲音
	CAudio::Instance()->Load(AUDIO_tap, "sounds\\10_tap.mp3");		// 載入編號10的聲音
	CAudio::Instance()->Load(AUDIO_felling, "sounds\\11_felling.mp3");		// 載入編號11的聲音
	CAudio::Instance()->Load(AUDIO_OVER, "sounds\\12_gameover.mp3");		// 載入編號12的聲音
	CAudio::Instance()->Load(AUDIO_doghit, "sounds\\13_doghit.mp3");		// 載入編號13的聲音
	CAudio::Instance()->Load(AUDIO_Hited, "sounds\\14_hit.mp3");		// 載入編號14的聲音
	CAudio::Instance()->Load(AUDIO_Eat, "sounds\\15_eat.mp3");		// 載入編號14的聲音
	// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
    //
	for (int i = 80; i <= 100; i++){
		ShowInitProgress(i);	// 接個前一個狀態的進度，此處進度視為i%
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT  = 0x25; // keyboard左箭頭
    const char KEY_UP    = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_SPACE = ' ';
	const char KEY_W = 87;
	const char KEY_S = 83;   // 按下S
	const char KEY_A = 65;
	const char KEY_D = 68;

	if (nChar == KEY_LEFT || nChar == KEY_A){
		gamemap.SetMovingLeft(true);
		Fist_Role.SetMovingLeft(true);
		if (Fist_Role.GetMovingLeft() == true
			&& Fist_Role.GetMovingUp() == false)Fist_Role.SetDir(1);
	}
	if (nChar == KEY_RIGHT || nChar == KEY_D){
		gamemap.SetMovingRight(true);
		Fist_Role.SetMovingRight(true);
		if (Fist_Role.GetMovingRight() == true
			&& Fist_Role.GetMovingUp() == false)Fist_Role.SetDir(2);
	}
	if (nChar == KEY_UP || nChar == KEY_W){
		gamemap.SetMovingUp(true);
		Fist_Role.SetMovingUp(true);
		if (Fist_Role.GetMovingDown() == false
			&& Fist_Role.GetMovingUp() == true)Fist_Role.SetDir(3);
	}
	if (nChar == KEY_DOWN || nChar == KEY_S){
		gamemap.SetMovingDown(true);
		Fist_Role.SetMovingDown(true);
		
	}
	if (Fist_Role.GetMovingUp() == true && Fist_Role.GetMovingDown() == true)Fist_Role.SetDir(0);
	else if (Fist_Role.GetMovingRight() == true && Fist_Role.GetMovingLeft() == true)Fist_Role.SetDir(0);
	else if (Fist_Role.GetMovingUp() == true)Fist_Role.SetDir(3);
	else if (Fist_Role.GetMovingDown() == true)Fist_Role.SetDir(4);
	else if (Fist_Role.GetMovingRight() == true)Fist_Role.SetDir(2);
	else if (Fist_Role.GetMovingLeft() == true)Fist_Role.SetDir(1);
	else Fist_Role.SetDir(0);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    const char KEY_LEFT  = 0x25; // keyboard左箭頭
    const char KEY_UP    = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_SPACE = ' ';
	const char KEY_R = 82;	//按下R
	const char KEY_ESC = 27;//按下ESC
	const char KEY_W = 87;
	const char KEY_S = 83;   // 按下S
	const char KEY_A = 65;
	const char KEY_D = 68;
		if (nChar == KEY_SPACE){		//keyboard空白鍵 
			Fist_Role.SetSPACE(true);			
		}
		if (nChar == KEY_LEFT || nChar == KEY_A)
		{	
			gamemap.SetMovingLeft(false);
			Fist_Role.SetMovingLeft(false);
		}
		if (nChar == KEY_RIGHT || nChar == KEY_D)
		{
			gamemap.SetMovingRight(false);
			Fist_Role.SetMovingRight(false);
		}
		if (nChar == KEY_UP || nChar == KEY_W){
			gamemap.SetMovingUp(false);
			Fist_Role.SetMovingUp(false);
		}
		if (nChar == KEY_DOWN || nChar == KEY_S)
		{
			gamemap.SetMovingDown(false);
			Fist_Role.SetMovingDown(false);
		}
		if (nChar == KEY_S){
		//	Savefile.Wfiles(".\\Save\\save.txt",&RoleState, &Item, &GameClock);
		}
		if (nChar == KEY_R){
			Savefile.Rfiles(".\\Save\\saveOP.txt",&RoleState, &Item, &GameClock);
		}
		if (nChar == KEY_ESC){
			if(PauseMenu.GetShow()==0)PauseMenu.SetShow(1);
			else PauseMenu.SetShow(0);
		}
		if (Fist_Role.GetMovingUp() == true && Fist_Role.GetMovingDown() == true)Fist_Role.SetDir(0);
		else if (Fist_Role.GetMovingRight() == true && Fist_Role.GetMovingLeft() == true)Fist_Role.SetDir(0);
		else if (Fist_Role.GetMovingUp() == true)Fist_Role.SetDir(3);
		else if (Fist_Role.GetMovingDown() == true)Fist_Role.SetDir(4);
		else if (Fist_Role.GetMovingRight() == true)Fist_Role.SetDir(2);
		else if (Fist_Role.GetMovingLeft() == true)Fist_Role.SetDir(1);
		else Fist_Role.SetDir(0);

		int ID = 0;
		for (int i = 0; i<18; i++){
			if (nChar == 49 + i){
				ID = Item.GetHaveItem(i).GetId();
				if (ID == 7 || ID == 9 || ID == 14 || ID == 15){
					CAudio::Instance()->Play(15, false);
				}
				if (ID == 7){
					Item.LostItem(7, 1);
					RoleState.SetHungyDegree(RoleState.GetHungyDegree() + 15);
					RoleState.SetLife(RoleState.GetLife() - 10);
				}
				else if (ID == 9){
					RoleState.SetHungyDegree(RoleState.GetHungyDegree() + 30);
					Item.LostItem(9, 1);
					RoleState.SetLife(RoleState.GetLife() + 5);
				}
				else if (ID == 14){
					RoleState.SetHungyDegree(RoleState.GetHungyDegree() + 15);
					RoleState.Setbrain(RoleState.Getbrain() + 5);
					Item.LostItem(14, 1);
				}
				else if (ID == 15){
					RoleState.SetHungyDegree(RoleState.GetHungyDegree() + 10);
					RoleState.Setbrain(RoleState.Getbrain() + 5);
					Item.LostItem(15, 1);
				}
			}
		}
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{	
	///點擊訊息框判斷
	if (Massage.GetStoryVariable() % 10 != 9 && Massage.GetIsShow()){
		Massage.SetStoryVariable(Massage.GetStoryVariable() + 1);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}///
	
	//判斷是否有足夠的素材
	int MakeHight = Make.GetY2() - Make.GetY1();
	if (point.x < Make.GetX2() &&
		point.x > Make.GetX1() &&
		point.y > Make.GetY1() &&
		point.y < Make.GetY1()+MakeHight/5){
		if (Make.GetMake_hud_tools_Show() == 2)Make.SetMake_hud_tools_Show(0);
		else Make.SetMake_hud_tools_Show(2);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	else if (
		point.x < Make.GetX2() &&
		point.x > Make.GetX1() &&
		point.y > Make.GetY1() + MakeHight / 5 &&
		point.y < Make.GetY1() + MakeHight / 5 * 2){
		if (Make.GetMake_hud_tools_Show() == 1)Make.SetMake_hud_tools_Show(0);
		else Make.SetMake_hud_tools_Show(1);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	else if (Make.MakeformHit(point.x, point.y)==0)
	{
		Make.SetMake_hud_tools_Show(0);
		Make.SetMake_AxesOnSow(0);
		Make.SetMake_torchfireOnSow(0); 
		Make.SetMake_campfireOnSow(0);
		Make.SetMake_spadeShow(0);
		Make.SetMake_pickaxeShow(0);
		Make.SetMake_hud_tools_Show(0);
	}
	//////////////////////////////////////////////////////////
	if (Make.GetMake_hud_tools_Show() == 1){//武器欄開啟
		if (point.x < Make.GetX2()+86 &&
			point.x > Make.GetX2() &&
			point.y > Make.GetY1() + 120  &&
			point.y < Make.GetY1() + 160 ){
			Make.SetMake_torchfireOnSow(0);
			Make.SetMake_campfireOnSow(0);
			Make.SetMake_AxesOnSow(1);
			Make.SetMake_spadeShow(0);
			Make.SetMake_pickaxeShow(0);
			CAudio::Instance()->Play(AUDIO_keydown, false);
		}
		if (point.x < Make.GetX2() + 86 &&
			point.x > Make.GetX2() &&
			point.y > Make.GetY1() + 170 &&
			point.y < Make.GetY1() + 210){
			Make.SetMake_torchfireOnSow(0);
			Make.SetMake_campfireOnSow(0);
			Make.SetMake_AxesOnSow(0);
			Make.SetMake_spadeShow(0);
			Make.SetMake_pickaxeShow(1);
			CAudio::Instance()->Play(AUDIO_keydown, false);
		}
		if (point.x < Make.GetX2() + 86 &&
			point.x > Make.GetX2() &&
			point.y > Make.GetY1() + 220 &&
			point.y < Make.GetY1() + 260){
			Make.SetMake_torchfireOnSow(0);
			Make.SetMake_campfireOnSow(0);
			Make.SetMake_AxesOnSow(0);
			Make.SetMake_spadeShow(1);
			Make.SetMake_pickaxeShow(0);
			CAudio::Instance()->Play(AUDIO_keydown, false);
		}
	}
	else if (Make.GetMake_hud_tools_Show() == 2){//火系列欄開啟
		if (point.x < Make.GetX2() + 86 &&
			point.x > Make.GetX2() &&
			point.y > Make.GetY1() + 120 &&
			point.y < Make.GetY1() + 160){
			Make.SetMake_torchfireOnSow(1);
			Make.SetMake_campfireOnSow(0);
			Make.SetMake_AxesOnSow(0);
			Make.SetMake_spadeShow(0);
			Make.SetMake_pickaxeShow(0);
			CAudio::Instance()->Play(AUDIO_keydown, false);
		}
		if (point.x < Make.GetX2() + 86 &&
			point.x > Make.GetX2() &&
			point.y > Make.GetY1() + 170 &&
			point.y < Make.GetY1() + 210){
			Make.SetMake_torchfireOnSow(0);
			Make.SetMake_campfireOnSow(1);
			Make.SetMake_AxesOnSow(0);
			Make.SetMake_spadeShow(0);
			Make.SetMake_pickaxeShow(0);
			CAudio::Instance()->Play(AUDIO_keydown, false);
		}
		if (point.x < Make.GetX2() + 86 &&
			point.x > Make.GetX2() &&
			point.y > Make.GetY1() + 220 &&
			point.y < Make.GetY1() + 260){
			Make.SetMake_torchfireOnSow(0);
			Make.SetMake_campfireOnSow(0);
			Make.SetMake_AxesOnSow(0);
			Make.SetMake_spadeShow(0);
			Make.SetMake_pickaxeShow(0);
		}
	}
	//////////////////////////////////////////////////////////
	if (Make.BuildHit(point.x, point.y) == 1 && Make.Enough(Item, 1,1,5,1) && Make.GetMake_AxesOnSow()>0){//製造斧頭
		Item.LostItem(1, 1);
		Item.LostItem(5, 1);
		Item.GetItem(3,99);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	else if (Make.BuildHit(point.x, point.y) == 1 && Make.Enough(Item,5,2,4,2) &&Make.GetMake_torchfireOnSow()>0){//製造火把
		Item.LostItem(5, 2);
		Item.LostItem(4, 2);
		Item.GetItem(8, 99);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	else if (Make.BuildHit(point.x, point.y) == 1 && Make.Enough(Item, 4, 3, 2, 2)  && Make.GetMake_campfireOnSow()>0){//製造營火
		Item.LostItem(4, 3);
		Item.LostItem(2, 2);
		Campfire[0].SetCampfire(Fist_Role.GetX1(), Fist_Role.GetY1(), &gamemap);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	else if (Make.BuildHit(point.x, point.y) == 1 && Make.Enough(Item, 5, 2, 1, 2) && Make.GetMake_pickaxeShow()>0){//製造鶴嘴鎬
		Item.LostItem(5, 2);
		Item.LostItem(1, 2);
		Item.GetItem(10, 99);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	else if (Make.BuildHit(point.x, point.y) == 1 && Make.Enough(Item, 5, 2, 1, 2) && Make.GetMake_spadeShow()>0){//製造鏟子
		Item.LostItem(5, 2);
		Item.LostItem(1, 2);
		Item.GetItem(11, 99);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	///////////////////////
	//點擊道具欄
	for (int i = 0; i<18; i++){
		if (point.x > Item.GetX1() + 16 + i * 36 &&
			point.x < Item.GetX1() + 47 + i * 36 &&
			point.y > Item.GetY1() + 16 &&
			point.y < Item.GetY1() + 49 ){
			Item.SetChoiceItem(i);
			CAudio::Instance()->Play(AUDIO_keydown, false);
		}
	}
	//////////////////////
	//點擊暫停
	if (PauseMenu.IsButton(point) == 1){
		if(PauseMenu.GetShow()==0)PauseMenu.SetShow(1);
		else PauseMenu.SetShow(0);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	else if (PauseMenu.IsButton(point) == 2){
		PauseMenu.SetShow(0);
		Savefile.Rfiles(".\\Save\\save.txt",&RoleState, &Item, &GameClock);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	else if (PauseMenu.IsButton(point) == 4){
		PauseMenu.SetShow(0);
		Savefile.Wfiles(".\\Save\\save.txt", &RoleState, &Item, &GameClock);
		GotoGameState(GAME_STATE_INIT);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	//////////////////////
	//死亡介面
	if (GameOver.IsButton(point) == 1){
		GameOver.SetShow(0);
		CAudio::Instance()->Play(AUDIO_keydown, false);
		GotoGameState(GAME_STATE_INIT);
	}
	else if (GameOver.IsButton(point) == 2){
		OnBeginState();
		GameOver.SetShow(0);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	//////////////////////
	//點擊小地圖
	if (minMap.IsButton(point) == 1){//小地圖點擊後事件
		if (minMap.GetShow()==1)minMap.SetShow(2);
		else minMap.SetShow(1);
		CAudio::Instance()->Play(AUDIO_keydown, false);
	}
	//////////////////////
	if (Campfire[0].IsAlive() && point.x>Campfire[0].GetX1() && point.x<Campfire[0].GetX2() + (Campfire[0].GetX2() - Campfire[0].GetX1()) * 2 &&
		point.y>Campfire[0].GetY1() && point.y < Campfire[0].GetY2()+(Campfire[0].GetY2()-Campfire[0].GetY1())*2){
		if (Item.GetChoiceItem() == 7){//烤兔子
			Item.GetItem(9, 1);
			Item.LostItem(7, 1);
			CAudio::Instance()->Play(9, false);
		}
	}
	else if (Item.GetChoiceItem() > 0 && Item.GetY1() > point.y){//丟東西
		Item.LostItem(Item.GetChoiceItem(), 999);
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{	
	Mouse.SetPoint(point.x, point.y);
	if (Campfire[0].IsAlive() && point.x>Campfire[0].GetX1() && point.x<Campfire[0].GetX2() + (Campfire[0].GetX2() - Campfire[0].GetX1()) * 2 &&
		point.y>Campfire[0].GetY1() && point.y < Campfire[0].GetY2() + (Campfire[0].GetY2() - Campfire[0].GetY1()) * 2){
		if (Item.GetChoiceItem() == 7){
			//手拿兔子顯示烤
			Item.SetfireYes(1);
		}
	}
	else if (Item.GetChoiceItem() != 0 && Item.GetY1()>point.y)Item.SetfireYes(2);
	if (Item.GetChoiceItem() == 0 || Item.GetY1()<point.y)Item.SetfireYes(0);
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	int ID = 0;
	for (int i = 0; i<18; i++){
		if (point.x > Item.GetX1() + 16 + i * 36 &&
			point.x < Item.GetX1() + 47 + i * 36 &&
			point.y > Item.GetY1() + 16 &&
			point.y < Item.GetY1() + 49){
			ID = Item.GetHaveItem(i).GetId();
			if (ID == 7 || ID == 9 || ID == 14 || ID == 15){
				CAudio::Instance()->Play(15, false);
			}
			if (ID == 7){
				Item.LostItem(7, 1);
				RoleState.SetHungyDegree(RoleState.GetHungyDegree() + 15);
				RoleState.SetLife(RoleState.GetLife() - 10);
			}
			else if (ID == 9){
				RoleState.SetHungyDegree(RoleState.GetHungyDegree() + 30);
				Item.LostItem(9, 1);
				RoleState.SetLife(RoleState.GetLife() + 5);
			}
			else if (ID == 14){
				RoleState.SetHungyDegree(RoleState.GetHungyDegree() + 15);
				RoleState.Setbrain(RoleState.Getbrain() + 5);
				Item.LostItem(14, 1);
			}
			else if (ID == 15){
				RoleState.SetHungyDegree(RoleState.GetHungyDegree() + 10);
				RoleState.Setbrain(RoleState.Getbrain() + 5);
				Item.LostItem(15, 1);
			}
		}
	}
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
	bool Roledraw = false;
    //
    //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
    //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
    //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
    //
	Sea.OnShow();
	gamemap.OnShow();				//地圖

	for (int c = 0; c < 90; c++){						//畫地圖上的兔子
		if (Rabbit[c].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&Rabbit[c], 55) &&
			Rabbit[c].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
		Rabbit[c].OnShow();
	}
	for (int c = 0; c < 90; c++){						//畫地圖上的兔子
		if (BlockDog[c].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&BlockDog[c], 55) &&
			BlockDog[c].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
		BlockDog[c].OnShow();
	}
	for (int j = 0; j < 90; j++){		//畫乾草
		if (Hay[j].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&Hay[j], 50)&&
			Hay[j].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
			Hay[j].OnShow();
	}
	for (int k = 0; k < 90; k++){		//畫樹枝
		if (Branch[k].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&Branch[k], 50)&&
			Branch[k].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
		Branch[k].OnShow();
	}
	for (int k = 0; k < 90; k++){		//畫帽子
		if (Hat[k].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&Hat[k], 50) &&
			Hat[k].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
		Hat[k].OnShow();
	}
	for (int k = 0; k < 90; k++){		//畫黃金
		if (Gold[k].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&Gold[k], 50) &&
			Gold[k].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
		Gold[k].OnShow();
	}
	for (int k = 0; k < 90; k++){		//畫紅羅波
		if (Carrot[k].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&Carrot[k], 50) &&
			Carrot[k].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
		Carrot[k].OnShow();
	}
	for (int k = 0; k < 90; k++){		//畫漿果
		if (Berry[k].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&Berry[k], 50) &&
			Berry[k].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
		Berry[k].OnShow();
	}
	for (int z = 0; z < 90; z++){						//畫地圖上的燧石
		if (flint[z].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&flint[z], 30)&&
			flint[z].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
		flint[z].OnShow();
	}
	for (int c = 0; c < 90; c++){						//畫地圖上的石頭
		if (stone[c].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&stone[c], 55)&&
			stone[c].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
		stone[c].OnShow();
	}
	for (int i = 0; i < 90; i++){						//畫地圖上的樹
		if (Tree[i].GetY2() >= Fist_Role.GetY2() &&
			Roledraw == false &&
			Fist_Role.HitOther(&Tree[i], 195)&&
			Tree[i].IsAlive()){
			Fist_Role.OnShow();
			Roledraw = true;
		}
		Tree[i].OnShow();
	}

	/////////////日夜系統
	
	if (GameClock.GetTime() > 1200){
		if (Item.GetHaveItem(15).GetId() == 8 || Fist_Role.HitOther(&Campfire[0], 160)){
			blackO.SetTopLeft(0, 0);
			blackO.ShowBitmap();
		}
		else{
			black100.SetTopLeft(0, 0);
			black100.ShowBitmap();
		}
	}
	for (int s = 0; s < 10; s++){		//畫營火
		if (GameClock.GetTime()<1200){
			Campfire[s].SetTime(true);
			Campfire[s].OnShow();
		}
		else{
			if (Fist_Role.HitOther(&Campfire[s], 160))Campfire[s].SetTime(true);
			else Campfire[s].SetTime(false);
			Campfire[s].OnShow();
		}
	}
	if (Roledraw == false)	Fist_Role.OnShow();
	if (GameClock.GetTime() > 800 && GameClock.GetTime() < 1200){//顯示要晚上
		text_willdark.SetTopLeft((SIZE_X - text_willdark.Width()/5) / 2, (SIZE_Y - text_willdark.Width()/5) / 2);
		text_willdark.ShowBitmap(0.2);
	}
	if (GameClock.GetTime() > 1500 && GameClock.GetTime() < 1600){//顯示要白天
		text_willsum.SetTopLeft((SIZE_X - text_willsum.Width() / 5) / 2, (SIZE_Y - text_willsum.Width() / 5) / 2);
		text_willsum.ShowBitmap(0.2);
	}
	if (GameClock.GetTime() == 1200){
		CAudio::Instance()->Stop(AUDIO_SunBGM);
		CAudio::Instance()->Play(AUDIO_NightBGM, false);
		CAudio::Instance()->Play(AUDIO_SunToNightBGM, false);
	}
	if (GameClock.GetTime() == 1600){
		CAudio::Instance()->Stop(AUDIO_NightBGM);
		CAudio::Instance()->Play(AUDIO_NightToSunBGM, false);
		CAudio::Instance()->Play(AUDIO_SunBGM, false);
	}

	
	if (GameOver.GetShow() == false){
		///角色狀態
		RoleState.OnShow();
		///介面
		GameClock.OnShow();
		Make.OnShow();
		Item.OnShow();
		minMap.OnShow(&gamemap,&Campfire[0],&Fist_Role);
		Item.ChaseMouse(Mouse.x, Mouse.y);//拖曳道具顯示
		Massage.OnShow();
		//暫停介面
		PauseMenu.OnShow();
	}
	GameOver.OnShow(GameClock.GetDays());
}
}