#include "pch.h"
#include "Planetary_Motion.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <cmath>

#define TOTAL_TIME 1.728e+6

std::vector<BODIES> planet_vec;
std::vector<double> pos_vec;
std::vector<BODIES> acceleration_vec;
int time_step = 3600;
double current_time = 0;
int num_bodies;

using namespace std;

void BODIES::get_planets() {

	string filename;
	ifstream planets;
	string line;
	int found;
	int count = 0;

	//Loading of file, while returning error if file not found:
	while (1)
	{
		cout << "Name of file to load: ";
		cin >> filename; //ask for user input of file name
		planets.open(filename); //open file by name input by user
		if (planets.is_open()) {
			cout << "File loaded!" << endl;
			break;
		}//end if
		else
		{
			cout << "File not found, try again" << endl;
		}//end else
	}//end while(1) loop

	getline(planets, line);

	found = line.find("#");
	line.erase(line.begin() + found, line.end());

	num_bodies = stoi(line);
	
	while (count < num_bodies) {

		BODIES new_planet;

		getline(planets, line);
		found = line.find("#");
		line.erase(line.begin() + found, line.end());
		new_planet.mass = stod(line);

		getline(planets, line);
		found = line.find("#");
		line.erase(line.begin() + found, line.end());
		new_planet.x = stod(line);

		getline(planets, line);
		found = line.find("#");
		line.erase(line.begin() + found, line.end());
		new_planet.y = stod(line);

		getline(planets, line);
		found = line.find("#");
		line.erase(line.begin() + found, line.end());
		new_planet.vx = stod(line);

		getline(planets, line);
		found = line.find("#");
		line.erase(line.begin() + found, line.end());
		new_planet.vy = stod(line);

		planet_vec.push_back(new_planet);
		pos_vec.push_back(new_planet.x);
		pos_vec.push_back(new_planet.y);
		
		count++;
	}
}

void BODIES::trajectory() {

	//get initial acceleration values for each body, push into acceleration vector
	for (unsigned int i = 0; i < num_bodies; i++) {
		
		BODIES acceleration;

		acceleration.an = 0;
		
		for (unsigned int j = 0; j < num_bodies; j++) {

			if (j == i) {
				continue;
			}
			else {
				acceleration.an += (G * planet_vec[j].mass) / pow(hypot(planet_vec[i].x, planet_vec[i].y) - hypot(planet_vec[j].x, planet_vec[j].y), 2);
			}
		}

		if (planet_vec[i].x == 0 && planet_vec[i].y == 0) {
			
			acceleration.ax = 0;
			acceleration.ay = 0;

		} else {

			acceleration.ax = -1 * (acceleration.an) * (planet_vec[i].x) / (hypot(planet_vec[i].x, planet_vec[i].y));
			acceleration.ay = -1 * (acceleration.an) * (planet_vec[i].y) / (hypot(planet_vec[i].x, planet_vec[i].y));
		}

		acceleration_vec.push_back(acceleration);

	}
	
	current_time += time_step;

	while (current_time <= TOTAL_TIME) {

		for (unsigned int i = 0; i < num_bodies; i++) {

			planet_vec[i].vx = planet_vec[i].vx + (acceleration_vec[i].ax * time_step);
			planet_vec[i].vy = planet_vec[i].vy + (acceleration_vec[i].ay * time_step);

			planet_vec[i].x = planet_vec[i].x + (planet_vec[i].vx * time_step);
			planet_vec[i].y = planet_vec[i].y + (planet_vec[i].vy * time_step);

			pos_vec.push_back(planet_vec[i].x);
			pos_vec.push_back(planet_vec[i].y);

			acceleration_vec[i].an = 0;

			for (unsigned int j = 0; j < num_bodies; j++) {

				if (j == i) {
					continue;
				}
				else {
					acceleration_vec[i].an += (G * planet_vec[j].mass) / pow(hypot(planet_vec[i].x, planet_vec[i].y) - hypot(planet_vec[j].x, planet_vec[j].y), 2);
				}
			}
			if (planet_vec[i].x == 0 && planet_vec[i].y == 0) {

				acceleration_vec[i].ax = 0;
				acceleration_vec[i].ay = 0;
			} 
			else {

				acceleration_vec[i].ax = -(acceleration_vec[i].an) * (planet_vec[i].x) / (hypot(planet_vec[i].x, planet_vec[i].y));
				acceleration_vec[i].ay = -(acceleration_vec[i].an) * (planet_vec[i].y) / (hypot(planet_vec[i].x, planet_vec[i].y));
			}
		}

		current_time += time_step;
	}
}

void BODIES::output() {
	
	//Creating of file to store calculation/result into:
	string output_name;
	cout << "What would you like to call output text file? (Exclude .txt)";
	cin >> output_name;
	output_name.append(".txt");
	ofstream outfile(output_name);

	//if file not loaded correctly, report it:
	//if (!outfile)
	//{
	//	std::cout << "oops, something went wrong! Ensure no spaces in filename. Try again. " << std::endl;
	//	return false;
	//}

	outfile << time_step << endl;
	outfile << num_bodies << endl;

	while (pos_vec.size() > 0) {

		for (unsigned int i = 0; i < 6; i++) {
			outfile.precision(4);
			outfile << scientific << pos_vec[i] << "    ";
		}

		pos_vec.erase(pos_vec.begin(), pos_vec.begin() + 6);
		outfile << endl;
	}

	outfile.close();
}

