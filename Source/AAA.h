class CM 
{
public:
	CM(int BlockX = 0, int BlockY = 0);
	void LoadBitmap();										// ���J�ϧ�
	void OnShow();											// �N�ϧζK��e��
private:
	bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
};