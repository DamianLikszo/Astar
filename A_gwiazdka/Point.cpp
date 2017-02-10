#include "stdafx.h"
#include "Point.h"
#include <math.h>
#include "Global.h"
#include <vector>
#include <iostream>

Point::Point(int nX, int nY, bool lWall)
{
	Point::isWall = lWall;
	Point::isStart = false;
	Point::isEnd = false;
	Point::isRoad = false;
	Point::pParent = nullptr;

	Point::_gScore = 0;
	Point::_hScore = 0;

	Point::_x = nX;
	Point::_y = nY;

	//debug
	Point::isClose = false;
	Point::isOpen = false;
}

int Point::GetX()
{
	return Point::_x;
}

int Point::GetY()
{
	return Point::_y;
}

double Point::GetFScore()
{
	return Point::_gScore + Point::_hScore;
}

double Point::GetGScore() 
{
	return Point::_gScore;
}

double Point::GetHScore()
{
	return Point::_hScore;
}

void Point::Oblicz_gScore() 
{
	if (Point::pParent == nullptr)
		Point::_gScore = 0;
	else
		Point::_gScore = Point::pParent->_gScore + 1 * nCOST;
	
	return;
}

void Point::Oblicz_hScore() 
{
	Point::_hScore = sqrt(pow(Point::_x - xEND, 2) + pow(Point::_y - yEND, 2));
	
	return;
}

std::vector<Point*> Point::MoiSasiedzi(Point *** aPoints) 
{
	std::vector<Point*> aSasiedzi;

	if( Point::_y - 1 >= 0 )		{ aSasiedzi.push_back(aPoints[_y - 1][_x]); }			// Gora
	if (Point::_y + 1 < nROWS)	{ aSasiedzi.push_back(aPoints[_y + 1][_x]); }				// Dol
	if (Point::_x - 1 >= 0 )		{ aSasiedzi.push_back(aPoints[_y][_x - 1]); }			// Lewa
	if (Point::_x + 1 < nCOLS)	{ aSasiedzi.push_back(aPoints[_y][_x + 1]); }				// Prawa

	return aSasiedzi;
}
 