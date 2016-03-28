class CM 
{
public:
	CM(int BlockX = 0, int BlockY = 0);
	void LoadBitmap();										// 載入圖形
	void OnShow();											// 將圖形貼到畫面
private:
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
};