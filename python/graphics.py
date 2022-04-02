import random

import OpenGL
from OpenGL.GL import *
from OpenGL.GLU import *

def drawUniverse(uni):
    for obj in uni.objs:
        glColor(random.uniform(0.5,1), random.uniform(0.5,1), random.uniform(0.5,1))
        glPushMatrix()
        glTranslate(obj.pos[0], obj.pos[1], obj.pos[2])
        glBegin(GL_POINTS)
        glVertex3f(0,0,0)
        glEnd()
        glPopMatrix()

