#pragma once
#include <vector>

#define PI			3.14159265358979
#define G			6.67384e-11      // gravitational constant in m^3/kg * s^2
#define M_EARTH		5.97219e24       // earth mass [kg]
#define R_EARTH		6.371e6        // earth radius [m]
#define T_GEOSTATIC	86164.0905l  // earth rotational period [s]

class BODIES {

private:
public:

	double x, y;
	double vx, vy;
	double an, ax, ay;
	double mass;
	
	void get_planets();
	void trajectory();
	void output();
};
