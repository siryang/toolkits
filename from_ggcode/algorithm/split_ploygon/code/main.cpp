#include "stdafx.h"
#include "split.h"

//Sample 1:
//input: poly = { {10, 10}, {300, 10}, {300, 50}, {10, 50} }, gridSize = 100.
//output:
//pieces[0] { {10, 10}, {100, 10}, {100, 50}, {10, 50} }
//pieces[0] { {100, 10}, {200, 10}, {200, 50}, {100, 50} }
//pieces[0] { {200, 10}, {300, 10}, {300, 50}, {200, 50} }
//num = 3

void makePoly1(Polygon& poly)
{
	poly.push_back(Point(10557, 8138));
	poly.push_back(Point(10141, 7941));
	poly.push_back(Point( 9875, 7651));
	poly.push_back(Point( 9861, 7173));
	poly.push_back(Point( 9800, 6880));
	poly.push_back(Point(10052, 6703));
	poly.push_back(Point( 9817, 6610));
	poly.push_back(Point(10150, 6534));
	poly.push_back(Point(10965, 6508));
	poly.push_back(Point(11662, 6511));
	poly.push_back(Point(11471, 6717));
	poly.push_back(Point(11046, 6612));
	poly.push_back(Point(10456, 6658));
	poly.push_back(Point(10277, 6801));
	poly.push_back(Point(10358, 7024));
	poly.push_back(Point(11011, 6911));
	poly.push_back(Point(11520, 6894));
	poly.push_back(Point(11945, 6933));
	poly.push_back(Point(11760, 7128));
	poly.push_back(Point(11266, 7164));
	poly.push_back(Point(10743, 7161));
	poly.push_back(Point(10465, 7184));
	poly.push_back(Point(10679, 7347));
	poly.push_back(Point(11049, 7353));
	poly.push_back(Point(11601, 7353));
	poly.push_back(Point(11980, 7626));
	poly.push_back(Point(12084, 7930));
	poly.push_back(Point(12174, 8183));
	poly.push_back(Point(11991, 8189));
	poly.push_back(Point(11659, 7891));
	poly.push_back(Point(11436, 7618));
	poly.push_back(Point(11000, 7654));
	poly.push_back(Point(10913, 7877));
	poly.push_back(Point(10855, 8107));
	poly.push_back(Point(10711, 8048));
	poly.push_back(Point(10563, 7637));
	poly.push_back(Point(10459, 7953));
}

void makePoly2(Polygon& poly)
{
	poly.push_back(Point(10231, 7910));
	poly.push_back(Point(8452, 7283	));
	poly.push_back(Point(9958, 6832	));
	poly.push_back(Point(11559, 7027));
	poly.push_back(Point(10537, 7304));
	poly.push_back(Point(11648, 7620));
	poly.push_back(Point(13317, 7087));
	poly.push_back(Point(12976, 7955));
}

void makePoly3(Polygon& poly)
{
	poly.push_back(Point(10, 10));
	poly.push_back(Point(300, 10));
	poly.push_back(Point(300, 50));
	poly.push_back(Point(300, 50));
}

void makePoly4(Polygon& poly)
{
	poly.push_back(Point(7301, 6040));
	poly.push_back(Point(7043, 5895));
	poly.push_back(Point(7013, 5679));
	poly.push_back(Point(6833, 5476));
	poly.push_back(Point(6847, 5197));
	poly.push_back(Point(6886, 4914));
	poly.push_back(Point(7036, 5063));
	poly.push_back(Point(7220, 5130));
	poly.push_back(Point(7317, 5091));
	poly.push_back(Point(6600, 4626));
	poly.push_back(Point(7126, 4621));
	poly.push_back(Point(6974, 4739));
	poly.push_back(Point(7322, 4900));
	poly.push_back(Point(7333, 4653));
	poly.push_back(Point(7851, 4651));
	poly.push_back(Point(7911, 4976));
	poly.push_back(Point(7552, 4990));
	poly.push_back(Point(7688, 5183));
	poly.push_back(Point(8142, 5027));
	poly.push_back(Point(8153, 4621));
	poly.push_back(Point(8602, 4630));
	poly.push_back(Point(8621, 5103));
	poly.push_back(Point(8271, 5296));
	poly.push_back(Point(7914, 5312));
	poly.push_back(Point(7423, 5285));
	poly.push_back(Point(7269, 5476));
	poly.push_back(Point(7278, 5685));
	poly.push_back(Point(7354, 5867));
	poly.push_back(Point(7402, 5432));
	poly.push_back(Point(7607, 5372));
	poly.push_back(Point(7633, 5789));
	poly.push_back(Point(7960, 5791));
	poly.push_back(Point(7660, 5384));
	poly.push_back(Point(8179, 5393));
	poly.push_back(Point(8160, 5893));
	poly.push_back(Point(8554, 5886));
	poly.push_back(Point(8805, 5215));
	poly.push_back(Point(9593, 5421));
	poly.push_back(Point(9402, 6280));
	poly.push_back(Point(8909, 6125));
	poly.push_back(Point(8960, 5476));
	poly.push_back(Point(8837, 5520));
	poly.push_back(Point(8655, 6278));
	poly.push_back(Point(8105, 6303));
	poly.push_back(Point(8003, 5950));
	poly.push_back(Point(7798, 6006));
	poly.push_back(Point(7948, 6234));
	poly.push_back(Point(7840, 6291));
	poly.push_back(Point(7651, 5976));
	poly.push_back(Point(7536, 5543));
	poly.push_back(Point(7455, 5554));
	poly.push_back(Point(7506, 6047));
	poly.push_back(Point(7165, 6305));
}

int main()
{
	Polygon poly;
		
	//makePoly1(poly);
	//makePoly2(poly);
	makePoly4(poly);

	Polygon* result;				
	size_t num;
	split(&poly, 300, &result, &num);
	output(result, num, "result");
	return 0;
}