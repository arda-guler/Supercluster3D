import numpy as np

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
