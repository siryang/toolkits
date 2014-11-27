#pragma once

typedef vector<Point> Polygon;

void split(const Polygon* poly, int gridSize, Polygon** pieces, size_t* num);
void output(const Polygon* polygons, size_t num, const char* dest);
