#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"

using namespace std;

class Tools
{
public:
	void ConsoleOut(string _output);
	bool RandBool();
	double RandomRange(double _val1, double _val2);
	double Lerp(double a, double b, float t);
	double RadiansToDegrees(double _a);
	double DegreesToRadians(double _a);
	double Distance(int _x1, int _y1, int _x2, int _y2);
	double constrainAngle(double x);
	double angleDiff(double a, double b);
	double bisectAngle(double a, double b, double speed);
	double AngleBetweenPoints(int _x1, int _y1, int _x2, int _y2);
	bool isBetween(float start, float end, float mid);
	double Lerp(double tar, double pos, double perc);
private:


};

