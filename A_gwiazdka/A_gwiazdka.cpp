// Damian Likszo
// grupa I ISI
// Studia Niestacjonarne

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "Point.h"
#include "Global.h"

using namespace std;

Point *** WczytajGrid();
void WypiszTablice(Point *** aPoints);
bool AStar(Point *** aPoints);
int NajnizszyFScore(vector<Point*> aTab);
bool CzyZawiera(vector<Point*> aTab, Point* pPoint);
void Rekonstrukcja(Point *** Points, Point* pPoint);

int main()
{
	Point*** aPoints;

	aPoints = WczytajGrid();

	if (aPoints != nullptr)
	{
		cout << "Wczytana trasa: " << endl;
		WypiszTablice(aPoints);
		cout << "\n\nNacisnij ENTER aby przejsc dalej.";
		getchar();

		if (AStar(aPoints))
		{
			system("cls");
			cout << "\n\nZnaleziona Trasa: " << endl;

			WypiszTablice(aPoints);
		}
		else
		{
			cout << "\n\nNie Znaleziono Trasy!" << endl;
		}
		

		cout << "\n\nCzyszczenie Pamieci..." << endl;

		// Czyszcenie tablicy
		for (int i = 0; i < nCOLS; i++)
		{
			delete[] aPoints[i];
		}
		delete[] aPoints;

	}
	else
		cout << "\nERROR! Nie wczytano pliku!" << endl;
	
	cout << "\n\nNacisnij ENTER aby zakonczyc.";
	getchar();

    return 0;
}

bool AStar(Point *** aPoints)
{
	vector<Point*> aClose;
	vector<Point*> aOpen;

	Point* pStart = aPoints[ySTART][xSTART];
	pStart->isStart = true;								
	Point* pEnd = aPoints[yEND][xEND];
	pEnd->isEnd = true;								

	//dodajemy element startowy
	pStart->Oblicz_gScore();
	pStart->Oblicz_hScore();
	aOpen.push_back(pStart);
	pStart->isOpen = true;								// debug

	while (aOpen.size() > 0)							// dopoki mymy obiekty brane pod uwage
	{
		int nElement = NajnizszyFScore(aOpen);			// bierzemy element o najnizszym fscore
		Point* pPunkt = aOpen[nElement];

		if (pPunkt == pEnd)
		{
			Rekonstrukcja(aPoints, pPunkt);				// rekonstrukcja
			return true;	
		}

		aOpen.erase(aOpen.begin() + nElement);
		pPunkt->isOpen = false;							// debug
		aClose.push_back(pPunkt);
		pPunkt->isClose = true;							// debug

		// Liczymy Sasiadow
		for each (Point* pSasiad in pPunkt->MoiSasiedzi(aPoints))
		{
			if( pSasiad->isWall )
				continue;

			if( CzyZawiera(aClose, pSasiad) )
				continue;

			double dTmp_gscore = pSasiad->GetGScore() + 1 * nCOST;
			bool lTmp_better = false;

			if (!CzyZawiera(aOpen, pSasiad))
			{
				aOpen.push_back(pSasiad);
				pSasiad->isOpen = true;
				pSasiad->Oblicz_hScore();
				lTmp_better = true;
			}
			else if (dTmp_gscore < pSasiad->GetGScore())
				lTmp_better = true;

			if (lTmp_better)
			{
				pSasiad->pParent = pPunkt;
				pSasiad->Oblicz_gScore();
			}

			if (lDEBUG)
			{
				//wyswietlanie
				system("cls");
				cout << endl;
				WypiszTablice(aPoints);
				cout << "\n\nLEGENDA: O - Punkty do rozpatrzenia | X - Punkty wykluczone" << endl;
			}
		}
	}

	return false;										// nie znaleziono trasy
}

void Rekonstrukcja(Point *** aPoints, Point * pPoint)
{
	pPoint->isRoad = true;

	if (pPoint->pParent != nullptr)
	{
		Rekonstrukcja(aPoints, pPoint->pParent);
	}

	return;
}

// Zwraca True jezeli wektor zawiera dany wskaznik
bool CzyZawiera(vector<Point*> aTab, Point* pSPoint)
{
	bool lRet = false;

	for each (Point* pPunkt in aTab)
	{
		if (pPunkt == pSPoint)
		{
			lRet = true;
			break;
		}
	}

	return lRet;
}

// Zwraca ktory element w wektorze ma najnizszy fscore
int NajnizszyFScore(vector<Point*> aTab)
{
	int nNum = 0;
	int nNumNajnizszy = 0;
	Point* pNPunkt = aTab[0];

	for each (Point* punkt in aTab)
	{
		if (punkt->GetFScore() < pNPunkt->GetFScore())
		{
			nNumNajnizszy = nNum;
			pNPunkt = punkt;
		}

		nNum++;
	}

	return nNumNajnizszy;
}

Point *** WczytajGrid() {
	Point *** aPoints = nullptr;
	int nTmp;

	// Tworzenie tablicy wskaŸników
	aPoints = new Point**[nROWS];

	for (int i = 0; i < nCOLS; i++)
		aPoints[i] = new Point*[nROWS];

	cout << "Wczytywanie danych z pliku\n" << endl;
	
	ifstream plik(cPLIK);

	for (int i = 0; i<nROWS; i++)
	{
		for (int j = 0; j<nCOLS; j++)
		{
			plik >> nTmp;
			aPoints[i][j] = new Point(j, i, nTmp == 5);
		}
	}

	plik.close();

	return aPoints;
}

void WypiszTablice(Point *** aPoints)
{
	for (int i = 0; i<nROWS; i++)
	{
		for (int j = 0; j<nCOLS; j++)
		{
			Point* punkt = aPoints[i][j];

			if (punkt->isStart)
				cout << "S";
			else if (punkt->isEnd)
				cout << "K";
			else if (punkt->isWall)
				cout << "5";
			else if (punkt->isRoad)
				cout << "3";
			else if (lDEBUG && punkt->isOpen)
				cout << "O";
			else if (lDEBUG && punkt->isClose)
				cout << "X";
			else
				cout << "0";

			cout << " ";
		}cout << "\n";
	}

	return;
}

