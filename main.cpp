#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <conio.h>

#include <GLFW/glfw3.h>

#include "constants.h"
#include "galaxy.h"
#include "Vector3D.h"
#include "cosmicvoid.h"
#include "universe.h"
#include "camera.h"
#include "Matrix3x3.h"

double randd(double min=0, double max=1) {
	return min + (double)(rand()) / ((double)(RAND_MAX / (max - min)));
}

Vector3D world2cam(camera cam, Vector3D obj_pos) {
	Vector3D rel_pos = obj_pos - cam.pos;
	Vector3D cam_x = cam.orient.v1();
	Vector3D cam_y = cam.orient.v2();
	Vector3D cam_z = cam.orient.v3();

	double z_dist = rel_pos.dot(cam_z);

	if (z_dist <= 0) {
		return Vector3D(0,0,-1);
	}

	double x_dist = rel_pos.dot(cam_x);
	double y_dist = rel_pos.dot(cam_y);

	double x_skew = -(x_dist / z_dist);
	double y_skew = -(y_dist / z_dist);

	return Vector3D(x_skew, y_skew, 0);
}

std::vector<galaxy> genesis(int num_galaxy=500, int num_void=10, double uni_size=30, double max_vel=0.1) {
	std::vector<galaxy> galaxy_list = {};
	std::vector<cosmicvoid> void_list = {};

	printf("Generating voids...\n");
	for (int i = 0; i < num_void; i++) {
		Vector3D n_pos = Vector3D(randd(-uni_size, uni_size), randd(-uni_size, uni_size), randd(0, uni_size*2));
		double n_size = randd(0, uni_size/0.7);
		cosmicvoid n_void = cosmicvoid(n_pos, n_size);
		void_list.push_back(n_void);
	}

	printf("Generating galaxies...\n");
	for (int i = 0; i < num_galaxy; i++) {
		Vector3D n_pos = Vector3D(randd(-uni_size, uni_size), randd(-uni_size, uni_size), randd(-uni_size, uni_size));
		Vector3D n_vel = Vector3D(randd(-max_vel, max_vel), randd(-max_vel, max_vel), randd(-max_vel, max_vel));
		double n_mass = randd(50, 350);
		galaxy n_gal = galaxy(n_pos, n_vel, n_mass);
		galaxy_list.push_back(n_gal);
	}

	printf("Cleaning up the universe...\n");
	for (auto &g:galaxy_list) {
		for (auto &v:void_list) {
			if (g.dist(v) < v.size) {
				std::remove(galaxy_list.begin(), galaxy_list.end(), g);
			}
		}
	}

	return galaxy_list;
}

int main() {

	// OPTIONS
	double dt_base = 500; // million years per iteration
	double dt = dt_base;
	bool auto_dt = true;

	printf("Initializing GLFW...\n");

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* mw = glfwCreateWindow(1000, 600, "SuperclusterC3D", NULL, NULL);

	if (!mw) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(mw);
	glfwSwapInterval(1);
	
	// setup view
	float ratio;
	int width, height;

	glfwGetFramebufferSize(mw, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glColor3d(1, 1, 1);
	glEnable(GL_POINT_SMOOTH);

	printf("Creating the universe...\n");
	srand(time(NULL));
	universe cosmos = universe();
	std::vector<galaxy> galaxies = genesis(100, 0, 30, 0);
	std::vector<Vector3D> accels;

	camera main_cam = camera(Vector3D(0, 0, -50));
	double cam_lin_speed = 0.5;
	double cam_rot_speed = 0.01;

	printf("Starting...\n\n");

	while (!glfwWindowShouldClose(mw)) {
		glfwPollEvents();

		// physics
		
		double high_accel = 0;
		accels.clear();

		printf("dt: %f\n", dt);

		// get user input
		if (_kbhit()) {
			int keycode = _getch();
			
			switch (keycode)
			{
			// linear movement
			case 119: // W
				main_cam.move_local(Vector3D(0, 0, 1) * cam_lin_speed);
				break;
			case 97: // A
				main_cam.move_local(Vector3D(1, 0, 0) * cam_lin_speed);
				break;
			case 115: // S
				main_cam.move_local(Vector3D(0, 0, -1) * cam_lin_speed);
				break;
			case 100: // D
				main_cam.move_local(Vector3D(-1, 0, 0) * cam_lin_speed);
				break;
			case 114: // R
				main_cam.move_local(Vector3D(0, -1, 0) * cam_lin_speed);
				break;
			case 102: // F
				main_cam.move_local(Vector3D(0, 1, 0) * cam_lin_speed);
				break;

			// rotation
			case 117: // U
				main_cam.rotate(main_cam.orient.v3(), cam_rot_speed);
				break;
			case 111: // O
				main_cam.rotate(main_cam.orient.v3(), -cam_rot_speed);
				break;
			case 141: // I
				main_cam.rotate(main_cam.orient.v1(), cam_rot_speed);
				break;
			case 107: // K
				main_cam.rotate(main_cam.orient.v1(), -cam_rot_speed);
				break;
			case 106: // J
				main_cam.rotate(main_cam.orient.v2(), cam_rot_speed);
				break;
			case 108: // L
				main_cam.rotate(main_cam.orient.v2(), -cam_rot_speed);
				break;

			default:
				__noop;
				break;
			}
		}

		// calculate all accelerations first
		for (int iter = 0; iter < galaxies.size();) {
			Vector3D accel = Vector3D();
			for (int iter2 = 0; iter2 < galaxies.size();) {
				if (iter != iter2) { // don't apply gravity on a galaxy by itself
									 // (I mean it is a thing, but simulated galaxies are rigid)

					accel += galaxies[iter].getGravityAccelBy(galaxies[iter2]);
					double accel_mag = galaxies[iter].getGravityAccelBy(galaxies[iter2]).mag();
					if (high_accel == 0 && accel_mag > 5E-7) {
						high_accel = accel_mag;
					}
					else if (high_accel > 0 && accel_mag > high_accel) {
						high_accel = accel_mag;
					}
				}
				iter2++;
			}
			accels.push_back(accel);
			// g.updateStateVectors(accel, dt); -- don't do this here
			// since some galaxies will be updated before others, the motion
			// calculations will not make sense especially at higher dts.
			iter++;
		}
		
		// apply the accelerations, now that all accelerations are
		// calculated for the frame
		for (int iter = 0; iter < galaxies.size();) {
			galaxies[iter].updateStateVectors(accels[iter], dt);
			iter++;
		}

		// apply expansion to universe with a cosmological model that makes a bit too much sense
		cosmos *= 1+(modified_cosmological_constant * dt);

		for (auto &g : galaxies) {
			g.pos *= 1+(modified_cosmological_constant * dt);
		}

		// drawing
		glClear(GL_COLOR_BUFFER_BIT);
		
		for (auto &g : galaxies) {
			// very basic 3D projection for static camera

			double cam_dist = main_cam.dist(g);
			double psize = 2/( (cam_dist/50) * (cam_dist/50) );
			Vector3D galaxy_screen_pos = world2cam(main_cam, g.pos);

			if (galaxy_screen_pos.z != -1){
				glPointSize(psize);

				//glColor3d(g.pos.z/30, 0, (30-g.pos.z)/30);
				glColor3d(1, 1, 1);

				glBegin(GL_POINTS);
				//glVertex3d((g.pos.x / (g.pos.z + 50)) * (1/ratio), g.pos.y / (g.pos.z + 50), 0);
				glVertex3d(galaxy_screen_pos.x, galaxy_screen_pos.y, galaxy_screen_pos.z);
				glEnd();
			}
		}

		//main_cam.rotate(Vector3D(1, 0, 0), 0.1);

		if(auto_dt){
			if (high_accel) {
				dt = dt_base / (high_accel / 5E-8);
			}
			else {
				dt = dt_base;
			}
		}
		
		// swap buffer and check for events
		glfwSwapBuffers(mw);
	}

	glfwDestroyWindow(mw);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}