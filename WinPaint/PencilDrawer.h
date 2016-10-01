#pragma once
class CPencilDrawer
{
public:
	static bool status;
	POINT checkedPoint = { 0, 0 };
	CPencilDrawer();
	~CPencilDrawer();
};

