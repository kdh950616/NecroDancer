#include "stdafx.h"
#include "rayCast.h"


rayCast::rayCast()
{
}


rayCast::~rayCast()
{
}

void rayCast::rayCasting(POINTFLOAT playerPos, int length, int angleNum)
{
	vvPoint _vvPoint;
	_vvPoint.clear();

	for (int i = 0; i < 360; i += angleNum)
	{
		vPoint tmpPoint;
		tmpPoint.clear();
		int brightness;
		for (int j = 0; j < length; j += 12)
		{
			
		}
	}

}
