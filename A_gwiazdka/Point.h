#pragma once

#include <vector>

class Point
{
public:
	bool isWall;						// Czy pole niedostepne
	bool isRoad;						// Czy pole jest droga
	bool isClose;						// Czy nalezy do listy close (debug)
	bool isOpen;						// Czy nalezy do listy open (debug)
	bool isStart;						// Czy punkt startowy(debug)
	bool isEnd;							// Czy punkt koncowu (debug)
	Point* pParent;						// Rodzic

	Point(int x, int y, bool wall);

	int GetX();
	int GetY();
	double GetFScore();
	double GetGScore();
	double GetHScore();

	void Oblicz_gScore();
	void Oblicz_hScore();
	std::vector<Point*> MoiSasiedzi(Point *** aPoints);

private:
	int _x;
	int _y;

	double _gScore;						// g() = odleglosc * koszt
	double _hScore;						// h() = metryka Euklidesa
};

