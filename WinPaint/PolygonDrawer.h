#pragma once

#define POLYGON_NO 0
#define POLYGON_BEGIN 1
#define POLYGON_PROCESS 2

class CPolygonDrawer
{
public:
	static char status;
	POINT checkedPoint;
	CPolygonDrawer();
	~CPolygonDrawer();
};

