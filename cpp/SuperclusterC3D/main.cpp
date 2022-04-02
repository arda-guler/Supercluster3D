#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>

#include <GLFW/glfw3.h>

#include "galaxy.h"
#include "Vector3D.h"
#include "cosmicvoid.h"

double randd(double min=0, double max=1) {
	return min + (double)(rand()) / ((double)(RAND_MAX / (max - min)));
}

std::vector<galaxy> genesis(int num_galaxy=500, int num_void=10, double uni_size=30, double max_vel=0.1) {
	std::vector<galaxy> galaxy_list = {};
	std::vector<cosmicvoid> void_list = {};

	printf("Generating voids...\n");
	for (int i = 0; i < num_void; i++) {
		Vector3D n_pos = Vector3D(randd(-uni_size, uni_size), randd(-uni_size, uni_size), randd(0, uni_size*2));
		double n_size = randd(-uni_size/0.7, uni_size/0.7);
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

	printf("Creating the universe...\n");
	std::vector<galaxy> galaxies = genesis(1500, 50, 30, 0.02);

	printf("Starting...\n");

	while (!glfwWindowShouldClose(mw)) {
		glfwPollEvents();

		// physics
		double dt = 0.1; // million years per iteration

		for (auto &g : galaxies) {
			Vector3D accel = Vector3D();
			for (auto &g2 : galaxies) {
				if (g != g2) {
					accel += g.getGravityAccelBy(g2);
				}
			}
			g.updateStateVectors(accel, dt);
		}

		// drawing
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (auto &g : galaxies) {
			// very basic 3D projection for static camera
			if (g.pos.z > -50) {

				int psize = (int)(2 / ((g.pos.z+50) * (g.pos.z + 50)));
				if (psize < 2)
					psize = 2;
				glPointSize(psize);

				glColor3d(g.pos.z/30, 0, (30-g.pos.z)/30);

				glBegin(GL_POINTS);
				glVertex3d((g.pos.x / (g.pos.z + 50)) * (1/ratio), g.pos.y / (g.pos.z + 50), 0);
				glEnd();
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