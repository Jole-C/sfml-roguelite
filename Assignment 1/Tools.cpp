//Tools.cpp : This cpp file holds a number of useful methods.
//

#include "Tools.h"
#include <iostream>
#include <sstream>
#include <random>
#include <functional>

using namespace std;

//Outputs text to the console
void Tools::ConsoleOut(string _output)
{
	cout << _output + "\n";
}

//Returns a random bool
bool Tools::RandBool()
{
	return rand() > (RAND_MAX / 2);
}

//Returns a true random number
double Tools::RandomRange(double _val1, double _val2)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(_val1, _val2);

	return dist(mt);
}

//Lerps from a to B at b time of t
double Tools::Lerp(double a, double b, float t) 
{
	return a * (1 - t) + b * t;
}

//Converts an angle from radians to degrees
//source: https://stackoverflow.com/questions/40776703/converting-from-radians-to-degrees
double Tools::RadiansToDegrees(double _a)
{
	return _a * (180.0 / 3.141592653589793238463);
}

//Converts an angle from degrees to radians
double Tools::DegreesToRadians(double _a)
{
	return _a * (3.141592653589793238463 / 180);
}

//returns distance between two points
double Tools::Distance(int _x1, int _y1, int _x2, int _y2)
{
	return sqrt(pow(_x2 - _x1, 2) + pow(_y2 - _y1, 2) * 1.0);
}

//Normalises angles
//source: https://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
double Tools::constrainAngle(double x){
	x = fmod(x + 180, 360);
	if (x < 0)
		x += 360;
	return x - 180;
}
double Tools::angleDiff(double a, double b) {
	double dif = fmod(b - a + 180, 360);
	if (dif < 0)
		dif += 360;
	return dif - 180;
}
double Tools::bisectAngle(double a, double b, double speed) {
	return constrainAngle(a + angleDiff(a, b) * speed);
}

//Return angle between two points
double Tools::AngleBetweenPoints(int _x1, int _y1, int _x2, int _y2)
{
	return atan2((_y2 - _y1), (_x2 - _x1));
}

//Check if angle is between points
//source: https://math.stackexchange.com/questions/1044905/simple-angle-between-two-angles-of-circle
bool Tools::isBetween(float start, float end, float mid) {
	end = (end - start) < 0.0f ? end - start + 360.0f : end - start;
	mid = (mid - start) < 0.0f ? mid - start + 360.0f : mid - start;
	return (mid < end);
}

//Lerps from tar to pos at a speed of perc
//Modified from source: https://github.com/NangiDev/pico-8-projects/blob/master/tech/lerp.p8
double Tools::Lerp(double tar, double pos, double perc)
{
	return (1 - perc) * tar + perc * pos;
}