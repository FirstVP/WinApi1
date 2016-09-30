#pragma once

#define PRINT_NO 0
#define PRINT_START 1
#define PRINT_SELECTING 2
#define PRINT_FINAL 3

class CPrintRectangleDrawer
{
public:
	static char status;
	POINT checkedPoint;
	CPrintRectangleDrawer();
	~CPrintRectangleDrawer();
};

