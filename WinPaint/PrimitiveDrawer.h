#pragma once
class CPrimitiveDrawer
{
public:
	CPrimitiveDrawer();
	virtual void Draw(HDC hdc, int x, int y, int x1, int y1) = 0;
	~CPrimitiveDrawer();
};

