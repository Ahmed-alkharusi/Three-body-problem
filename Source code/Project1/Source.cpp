/*
=============================================================================
 Solving the three - body problem numerically using the RK4
 =============================================================================
Updated on Aug 12 2020
@author: Ahmed Alkharusi
I used the olcPixelGameEngine to generate the graphics.
The "olcPixelGameEngine.h" is a single header file that enables us to draw graphics.
This is created by javidx9 (OneLoneCoder).
please download it from his repo
https://github.com/OneLoneCoder/olcPixelGameEngine
*/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <array>
#include <cstdlib>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Array.h"
#include "Matrix.h"
std::string print_info(double t, double step_size, double m1, double m2, double m3);
void print_about();
std::string print_menu();
Matrix deriv(Matrix obj_active, Matrix obj_passive1, double mp1, Matrix obj_passive2, double mp2);
void rk4(Matrix(*deriv)(Matrix, Matrix, double, Matrix, double), Matrix& obj_active, Matrix& obj_passive1, double mp1, Matrix& obj_passive2, double mp2, double h);
class ThreeBodyProblem : public olc::PixelGameEngine
{public:
	ThreeBodyProblem()
	{
		sAppName = "Three Body Problem (Simulation)";
	}

private:
	double step_size{ 1.0 / 1000.0 };//step size for the RK4 method

	//initial conditions (play around with these)
	double t{ 0 };
	double m1{ 1 };
	double m2{ 1 };
	double m3{ 1 };
	//----------------{x,vx}----------------------------{y,vy}-------------------------------
	Matrix obj_1{ Array{0.97000436,0.93240737/2},			Array{-0.24308753,0.86473146 / 2} };
	Matrix obj_2{ Array{-0.97000436,0.93240737 / 2},		Array{0.24308753,0.86473146/2} };
	Matrix obj_3{ Array{0,-0.93240737},						Array{0,-0.86473146} };
	Matrix temp1 = obj_1;
	Matrix temp2 = obj_2;

	int temp = 1;
	const int zoom = 300;
	size_t counter = 0;
	size_t max_points = 999;
	//-----------------------draw the path--------------
	std::array<double, 1000> obj_1_x, obj_2_x, obj_3_x, obj_1_y, obj_2_y, obj_3_y;
	//-----CLOLOURS
	std::vector<int> vec_colour_1{ 0, 0, 0 };
	std::vector<int> vec_colour_2{ 0, 0, 255 };
	std::vector<int> vec_colour_3{ 255, 0, 0 };


public:
	bool OnUserCreate() override
	{

		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{

		int x_shift = ScreenWidth() / 2 - 200;
		int y_shift = ScreenHeight() / 2;
		if (temp == 1) {
			Clear(olc::WHITE);
		}
		else if (temp == 2)
		{
			Clear(olc::RED);
		}
		else if (temp == 3)
		{
			Clear(olc::BLACK);
		}
		else if (temp == 4) {
			Clear(olc::BLUE);
		}

		temp1 = obj_1;
		temp2 = obj_2;
		rk4(deriv, obj_1, obj_2, m2, obj_3, m3, step_size);
		rk4(deriv, obj_2, temp1, m1, obj_3, m3, step_size);
		rk4(deriv, obj_3, temp1, m1, temp2, m2, step_size);

		FillCircle((obj_1.arr_x).position * zoom + x_shift, (obj_1.arr_y).position * zoom + y_shift, m1*20, olc::Pixel(vec_colour_1[0],vec_colour_1[1],vec_colour_1[2]));
		FillCircle((obj_2.arr_x).position * zoom + x_shift, (obj_2.arr_y).position * zoom + y_shift, m2*20, olc::Pixel(vec_colour_2[0], vec_colour_2[1], vec_colour_2[2]));
		FillCircle((obj_3.arr_x).position * zoom + x_shift, (obj_3.arr_y).position * zoom + y_shift, m3*20, olc::Pixel(vec_colour_3[0], vec_colour_3[1], vec_colour_3[2]));
		if (counter == max_points + 1)
			counter = 0;
		obj_1_x[counter] = (obj_1.arr_x).position;
		obj_2_x[counter] = (obj_2.arr_x).position;
		obj_3_x[counter] = (obj_3.arr_x).position;

		obj_1_y[counter] = (obj_1.arr_y).position;
		obj_2_y[counter] = (obj_2.arr_y).position;
		obj_3_y[counter] = (obj_3.arr_y).position;


		for (size_t i{ 0 };i <= max_points;i++) {
			FillCircle((obj_1_x[i]) * zoom + x_shift, (obj_1_y[i]) * zoom + y_shift, m1 , olc::Pixel(vec_colour_1[0], vec_colour_1[1], vec_colour_1[2]));
			FillCircle((obj_2_x[i]) * zoom + x_shift, (obj_2_y[i]) * zoom + y_shift, m2 , olc::Pixel(vec_colour_2[0], vec_colour_2[1], vec_colour_2[2]));
			FillCircle((obj_3_x[i]) * zoom + x_shift, (obj_3_y[i]) * zoom + y_shift, m3, olc::Pixel(vec_colour_3[0], vec_colour_3[1], vec_colour_3[2]));

		}
		if (GetKey(olc::Key::B).bHeld) {
			bool done = false;
			do {
				std::cout << " \n Please select a clour from the list:\n 1. WHITE \n 2. RED \n 3. BLACK \n 4. BLUE \n";
				std::cin >> temp;
				if (temp == 1 || temp == 2 || temp == 3 || temp == 4) {
					done = true;
				}
			} while (!done);
		}
		counter += 1;
		if(!GetKey(olc::Key::H).bHeld) {
			DrawString(ScreenWidth() - 500, 50, print_menu(), olc::DARK_BLUE, 2);
			DrawString(ScreenWidth() - 500, ScreenHeight() - 500, print_info(t, step_size,m1 ,m2,m3), olc::DARK_BLUE, 2);
		}
		if (GetKey(olc::Key::A).bHeld) print_about();
		if (GetKey(olc::Key::C).bHeld)
		{
			bool done = false;
			do {
				std::cout << "\n See https://www.rapidtables.com/web/color/RGB_Color.html\n\nPlease enter the 1st RGB colour:\n THE VALUES MUST BE IN THE RANGE 0 - 255\ne.g. 255 255 0\n ";
				std::cin >> vec_colour_1[0] >> vec_colour_1[1] >> vec_colour_1[2];
				if (vec_colour_1[0] <= 255 && vec_colour_1[0] >= 0 && vec_colour_1[1] <= 255 && vec_colour_1[1] >= 0 && vec_colour_1[2] <= 255 && vec_colour_1[2] >= 0)
					done = true;
			} while (!done);
			done = false;
			do {
				std::cout << "\n Please enter the -----2nd---- RGB colour:\n THE VALUES MUST BE IN THE RANGE 0 - 255\ne.g. 255 255 0\n ";
				std::cin >> vec_colour_2[0] >> vec_colour_2[1] >> vec_colour_2[2];
				if (vec_colour_2[0] <= 255 && vec_colour_2[0] >= 0 && vec_colour_2[1] <= 255 && vec_colour_2[1] >= 0 && vec_colour_2[2] <= 255 && vec_colour_2[2] >= 0)
					done = true;
			} while (!done);
			done = false;
			do {
				std::cout << "\n Please enter the -----3rd----- RGB colour:\n THE VALUES MUST BE IN THE RANGE 0 - 255\ne.g. 255 255 0\n ";
				std::cin >> vec_colour_3[0] >> vec_colour_3[1] >> vec_colour_3[2];
				if (vec_colour_3[0] <= 255 && vec_colour_3[0] >= 0 && vec_colour_3[1] <= 255 && vec_colour_3[1] >= 0 && vec_colour_3[2] <= 255 && vec_colour_3[2] >= 0)
					done = true;
			} while (!done);

		}
		if (GetKey(olc::Key::M).bHeld)
		{
			bool done = false;
			do {
				std::cout << " Please enter the masses (must be positive) separated by a space\n e.g. 1 4 2\n";
				std::cin >> m1 >> m2 >> m3;
				if (m1 >= 0 && m2 >= 0 && m3 >= 0)
					done = true;
			} while (!done);

			done = false;
		}
		if (GetKey(olc::Key::D).bHeld)
		{
			bool done = false;
			do {
				std::cout << " Please enter the step size (must be positive) \n e.g. 0.0001 \n";
				std::cin >> step_size;
				if (step_size >= 0)
					done = true;
			} while (!done);

			done = false;
		}
		if (GetKey(olc::Key::S).bHeld)
		{
			DrawLine(x_shift, ScreenHeight()/2, x_shift + zoom, ScreenHeight() / 2, olc::DARK_BLUE);
			DrawLine(x_shift, ScreenHeight() / 2, x_shift, ScreenHeight() / 2 - zoom, olc::DARK_BLUE);
			DrawString(x_shift +10, ScreenHeight() / 2 +10, "(0,0)", olc::DARK_BLUE,2);
			DrawString(x_shift + zoom+10 , ScreenHeight() / 2 + 10, "(1,0)", olc::DARK_BLUE,2);
			DrawString(x_shift + 10, ScreenHeight() / 2 - zoom-10, "(0,1)", olc::DARK_BLUE,2);
		}
		if (GetKey(olc::Key::I).bHeld)
		{
			bool done = false;
			do{
			std::cout << " Please enter the initial conditions for the first object \n x-position (between -3.2 and 3.2) x-speed y-position (between -1.7 and 1.7) y-speed \n e.g. 1 0.2 1.3 3 "<<std::endl;
			std::cin >> obj_1.arr_x.position >> obj_1.arr_x.speed >> obj_1.arr_y.position >> obj_1.arr_y.speed;
			if (std::abs(obj_1.arr_x.position) <= 3.2 && std::abs(obj_1.arr_y.position) <= 1.7) {
				done = true;
				obj_1.arr_y.position *= -1;
			}
			} while (!done);
			done = false;
			do {
				std::cout << "\n Now for the ---2nd-- object \n x-position (between -3.2 and 3.2) x-speed y-position (between -1.7 and 1.7) y-speed \n e.g. 1 0.2 1.3 3 " << std::endl;
				std::cin >> obj_2.arr_x.position >> obj_2.arr_x.speed >> obj_2.arr_y.position >> obj_2.arr_y.speed;
				if (std::abs(obj_2.arr_x.position) <= 3.2 && std::abs(obj_2.arr_y.position) <= 1.7) {
					done = true;
					obj_2.arr_y.position *= -1;
				}

			} while (!done);
			done = false;
			do {
				std::cout << " \n Now for the ---3rd-- object \n x-position  \n x-position (between -3.2 and 3.2) x-speed y-position (between -1.7 and 1.7) y-speed \n e.g. 1 0.2 1.3 3 " << std::endl;
				std::cin >> obj_3.arr_x.position >> obj_3.arr_x.speed >> obj_3.arr_y.position >> obj_3.arr_y.speed;
				if (std::abs(obj_3.arr_x.position) <= 3.2 && std::abs(obj_3.arr_y.position) <= 1.7) {
					done = true;
					obj_3.arr_y.position *= -1;
				}
			} while (!done);

		}

		t += step_size;

		return true;
	}
};



int main() {
	ThreeBodyProblem object;
	int resolution_height{ 1080 };
	int resolution_width{ 1920 };
	if (object.Construct(resolution_width, resolution_height, 1, 1))
		object.Start();

	return 0;
}
Matrix deriv(Matrix obj_active, Matrix obj_passive1, double mp1, Matrix obj_passive2, double mp2) {
	double v_x = (obj_active.arr_x).speed;
	double v_y = (obj_active.arr_y).speed;
	double xo = (obj_active.arr_x).position;
	double yo = (obj_active.arr_y).position;
	double xp1 = (obj_passive1.arr_x).position;
	double yp1 = (obj_passive1.arr_y).position;
	double xp2 = (obj_passive2.arr_x).position;
	double yp2 = (obj_passive2.arr_y).position;

	double acc_x = -((mp1 * (xo - xp1) / pow(pow(xo - xp1, 2.0) + pow(yo - yp1, 2.0), 3.0 / 2.0)) + (mp2 * (xo - xp2) / pow(pow(xo - xp2, 2.0) + pow(yo - yp2, 2.0), 3.0 / 2.0)));
	double acc_y = -((mp1 * (yo - yp1) / pow(pow(xo - xp1, 2.0) + pow(yo - yp1, 2.0), 3.0 / 2.0)) + (mp2 * (yo - yp2) / pow(pow(xo - xp2, 2.0) + pow(yo - yp2, 2.0), 3.0 / 2.0)));

	return Matrix{ Array{v_x,acc_x}, Array{v_y,acc_y} };
}
void rk4(Matrix(*deriv)(Matrix, Matrix, double, Matrix, double), Matrix& obj_active, Matrix& obj_passive1, double mp1, Matrix& obj_passive2, double mp2, double h) {
	Matrix k1 = deriv(obj_active, obj_passive1, mp1, obj_passive2, mp2);
	Matrix k2 = deriv(obj_active + h / 2, obj_passive1, mp1, obj_passive2, mp2);
	Matrix k3 = deriv(obj_active + h / 2, obj_passive1, mp1, obj_passive2, mp2);
	Matrix k4 = deriv(obj_active + h, obj_passive1, mp1, obj_passive2, mp2);
	obj_active = obj_active + ((k1 + (k2 * 2) + (k3 * 2) + k4)) * (h / 6);
}
std::string print_menu() {
	std::ostringstream oss;
	oss << std::setw(15)<<std::left<<"MENU"<<"| KEY (HOLD) " << "\n\n"
		<< std::setw(15) << std::left <<std::setfill('_')<<"" << "\n\n" << std::setfill(' ')
		<< std::setw(15) << std::left <<"CHANGE INITIAL CONDITOINS"<<"| I " << "\n\n"
		<< std::setw(15) << std::left << "SHOW AXES" << "| S " << "\n\n"
		<< std::setw(15) << std::left << "HIDE TEXT" << "| H " << "\n\n"
		<< std::setw(15) << std::left << "CHANGE BACKGROUND COLOUR" << "| B " << "\n\n"
		<< std::setw(15) << std::left << "CHANGE STEP SIZE" << "| D " << "\n\n"
		<< std::setw(15) << std::left << "CHANGE OBJECTS COLOURS" << "| C " << "\n\n"
		<< std::setw(15) << std::left << "CHANGE MASSES"<<"| M " << "\n\n"
		<< std::setw(15) << std::left << "ABOUT" << "| A " << "\n\n"
		<< std::setw(15) << std::left << std::setfill('_') << "" << "\n\n" << std::setfill(' ')
		<< std::setw(15) << std::left << "NOTE: THIS WILL APEAR IN" << "\n\n"
		<< std::setw(15) << std::left << "A SEPARATE WINDOW!" << "\n\n"
		<<std::endl;
	std::string str = oss.str();
	return str;
}
std::string print_info(double t, double step_size, double m1, double m2, double m3) {
	std::ostringstream oss;
	oss << std::setw(8) << std::left << std::setfill('_') << "" << "\n\n" << std::setfill(' ')
		<< std::setw(8) << std::setprecision(6) <<std::left << "STEP SIZE: " << step_size  << "\n\n"
		<< std::setw(8) << std::setprecision(2) << std::left << "TIME: " << t << " arb." << "\n\n"
		<< std::setw(8) << std::left << "MASS_1: "<< m1 << " arb." << "\n\n"
		<< std::setw(8) << std::left << "MASS_2:" << m2 << " arb." << "\n\n"
		<< std::setw(8) << std::left << "MASS_3:" << m3 << " arb." << "\n\n"

		<< std::endl;
	std::string str = oss.str();
	return str;
}
void print_about() {
	std::cout << "\n\n=============================================================================\nThree-Body Problem simulation using Runge Kutta method(RK4)\n========================================================================================\nUpdated on Aug 11 2020\n@author: Ahmed Alkharusi\nI used the olcPixelGameEngine to generate the graphics.\nThe olcPixelGameEngine.h is a single header file that enables us to draw graphics.\nThis is created by javidx9(OneLoneCoder).\nplease download it from his repo\nhttps ://github.com/OneLoneCoder/olcPixelGameEngine " << std::endl;
}

/*
=============================================================================
 Please check the answers!!!
 =============================================================================
References:
#Implementing the RK4 method in Python
https ://youtu.be/mqoqAovXxWA
by Prof.Niels Walet

#for the initial conditions see
https ://doi.org/10.1093/pasj/psy057
and
https ://arxiv.org/abs/math/0011268
*/
