## UNLESS OTHERWISE SPECIFIED:
## Length/distance units are in Mpc
## Mass units are in 1E40 kg
## Velocity units are in km s-1
## Time units are in 100,000 Earth years (a year being nearly equivalent to the length of year 2000)

import numpy as np
import random
import time as tmod
import keyboard as kbd

import OpenGL
from OpenGL.GL import *
from OpenGL.GLU import *
import glfw

grav_const = 4.30091*(10**(+1)) # Mpc (1E40)kg-1 (km/s)2

def pc2km(x):
    """Convers parsecs to kilometres."""
    return x * 3.0857*(10**13)

def pc2ly(x):
    """Converts parsecs to light years."""
    return x * 3.26156

def pc2pm(x):
    """Converts parsecs to petametres (1E15 metre)."""
    return x * 3.0857

def sm2_1E30(x):
    """Converts solar mass to 1E30 kilograms"""
    return x / 1.988

def dist(obj1, obj2):
    """Gets 3D distance between two objects."""
    rel_pos = obj2.pos - obj1.pos
    return np.linalg.norm(rel_pos)

def get_direction_from_to(frm, to):
    """Gets a unit vector in the direction from argument 1 to argument 2."""
    rel_pos = to.pos - frm.pos
    return rel_pos/dist(frm,to)

class universe:
    def __init__(self,
                 matrix=np.array([[1,0,0],[0,1,0],[0,0,1]]),
                 objs=[]):
        
        self.matrix = matrix
        self.objs = objs

    def get_volume(self):
        # please only use this with diagonal matrices, thank you
        return self.matrix[0][0] * self.matrix[1][1] * self.matrix[2][2]

    def get_cosmological_const(self):
        return 0.00005 # idk

class void:
    def __init__(self, pos, size):
        self.pos = pos
        self.size = size

class galaxy:
    def __init__(self, pos, vel, mass):
        self.pos = pos
        self.vel = vel
        self.mass = mass

    def get_gravity_accel_towards(self, obj):
        if obj == self:
            return 0
        else:
            # too close, apply max gravity
            # (So you know how gravity no longer increases but decreases when you go underground on Earth? Yeah, similar things.)
            if dist(self, obj) < 0.03:
                # F_grav is in (1E40)kg * km * s-2 
                F_grav = (get_direction_from_to(self, obj) * grav_const * self.mass * obj.mass) / (0.03 * pc2km(0.03))

                # a_grav is in km * s-2
                a_grav = F_grav / self.mass

            # not too close yet, apply regular gravity
            else:
                # F_grav is in (1E40)kg * km * s-2 
                F_grav = (get_direction_from_to(self, obj) * grav_const * self.mass * obj.mass) / (dist(self, obj) * pc2km(dist(self, obj)))

                # a_grav is in km * s-2
                a_grav = F_grav / self.mass
            
            return a_grav

    def update_vel(self, accel, dt):
        # vel is in km s-1
        # accel is in km s-2
        # dt is in (100,000 years)

        # every 100,000 year is 3.1536*(10**12) seconds
        self.vel += accel * dt * 3.1536*(10**12)

    def update_pos(self, dt):
        # pos is in Mpc
        # vel is in km s-1
        # dt is in (100,000 years)

        # every 100,000 year is 3.1536*(10**12) seconds
        # every Mpc is 3.0857*(10**19) km
        
        self.pos += (self.vel/(3.0857*(10**19))) * (dt * 3.1536*(10**12))

def drawUniverse(uni):
    for obj in uni.objs:
        glColor(random.uniform(0.5,1), random.uniform(0.5,1), random.uniform(0.5,1))
        glPushMatrix()
        glTranslate(obj.pos[0], obj.pos[1], obj.pos[2])
        glBegin(GL_POINTS)
        glVertex3f(0,0,0)
        glEnd()
        glPopMatrix()

def main():
    print("INITALIZING...")
    glfw.init()
    window_x = 1000
    window_y = 600
    window = glfw.create_window(window_x,window_y,"CosmoSim3D", None, None)
    glfw.set_window_pos(window,50,50)
    glfw.make_context_current(window)
    
    gluPerspective(70, window_x/window_y, 0.01, 1000)
    glPointSize(2)

    def genesis():
        attempt = 0
        while True:
            print("Attempting galaxy & void generation...")
            # how much you want to stress-test your machine
            num_galaxies = 500

            # how much you want to cut some slack
            num_voids = 10

            universe_size = 25
            print("Generating galaxies...")
            # generate galaxies
            galaxies = []
            for i in range(num_galaxies):
                new_pos = np.array([random.uniform(-universe_size,universe_size), random.uniform(-universe_size,universe_size), random.uniform(-universe_size,universe_size)])
                new_vel = np.array([random.uniform(-5000,5000), random.uniform(-5000,5000), random.uniform(-5000,5000)])
                new_mass = 200 + random.uniform(-150, 150)
                new_galaxy = galaxy(new_pos, new_vel, new_mass)
                galaxies.append(new_galaxy)

            print("Creating voids...")
            # generate voids
            for i in range(num_voids):
                void_pos = np.array([random.uniform(-universe_size,universe_size), random.uniform(-universe_size,universe_size), random.uniform(-universe_size,universe_size)])
                void_size = random.uniform(5, universe_size*1.2)
                v_c = void(void_pos, void_size)

                for g in galaxies:
                    if dist(g, v_c) < v_c.size:
                        galaxies.remove(g)
                        del g

                del v_c

            if (num_galaxies/5 < len(galaxies) < num_galaxies/2) or (attempt > 20):
                break

            attempt += 1

        print("Final number of galaxies:", len(galaxies))
        return galaxies

    print("Creating spacetime...")
    # generate spacetime
    cosmos = universe(objs=genesis())
    
    glTranslate(0,0,-70)

    print("Starting...")
    time = 0
    dt = 10
    cycle = 0
    while True:

        if kbd.is_pressed("j"):
            glTranslate(0.1,0,0)
        if kbd.is_pressed("l"):
            glTranslate(-0.1,0,0)
        if kbd.is_pressed("u"):
            glTranslate(0,-0.1,0)
        if kbd.is_pressed("o"):
            glTranslate(0,0.1,0)
        if kbd.is_pressed("i"):
            glTranslate(0,0,0.1)
        if kbd.is_pressed("k"):
            glTranslate(0,0,-0.1)

        if kbd.is_pressed("w"):
            glRotate(1,1,0,0)
        if kbd.is_pressed("s"):
            glRotate(1,-1,0,0)
        if kbd.is_pressed("a"):
            glRotate(1,0,1,0)
        if kbd.is_pressed("d"):
            glRotate(1,0,-1,0)
        if kbd.is_pressed("q"):
            glRotate(1,0,0,1)
        if kbd.is_pressed("e"):
            glRotate(1,0,0,-1)

        glRotate(0.05, 0, -1, 0)

        high_grav = 0
        
        glfw.poll_events()
        
        # don't ask me
        dark_energy = cosmos.get_volume() * cosmos.get_cosmological_const()
        cosmos.matrix = cosmos.matrix * (1+dark_energy)

        # the universe expanded, so things get farther apart
        for obj_c in cosmos.objs:
            obj_c.pos *= (1+dark_energy)
        
        for obj_c in cosmos.objs:
            net_accel = 0
            # apply gravity to galaxy by every other galaxy
            for obj_o in cosmos.objs:
                net_accel += obj_c.get_gravity_accel_towards(obj_o)
                
            obj_c.update_vel(net_accel, dt)

            if (np.linalg.norm(net_accel) > 10**(-10)) and (np.linalg.norm(net_accel) > high_grav):
                high_grav = np.linalg.norm(net_accel)
                
            # update position
            obj_c.update_pos(dt)

        # draw things on screen
        if cycle % 1 == 0:
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
            drawUniverse(cosmos)
            glfw.swap_buffers(window)
            
        time += dt
        cycle += 1
        #tmod.sleep(0.01)

        if high_grav:
            dt = max(5 / (high_grav / 10**(-10)), 0.05)
        else:
            dt = 10

        print("\ndt:",dt)

main()
