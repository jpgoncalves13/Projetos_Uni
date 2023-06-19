# Node class
class Node:
    # Constructor of a node given the coordinates and the velocity
    def __init__(self, n, vel):
        self.coord = n  # n is tuple
        self.vel = vel  # vel is tuple

    # Returns the string representation of the node to be read 'friendly'
    def __str__(self):
        return "(Pos=" + str(self.coord) + ", Vel=" + str(self.vel) + ")"

    # Returns 'official' representation of the object
    def __repr__(self):
        return "(Pos=" + str(self.coord) + ", Vel=" + str(self.vel) + ")"

    # Get the coordinates of a node
    def get_coord(self):
        return self.coord

    # Get the speed of a node
    def get_speed(self):
        return self.vel

    # Update node coordinates
    def set_coord(self, n):
        self.coord = n

    # Update node speed
    def set_speed(self, vel):
        self.vel = vel

    # Method used to compare two nodes, in this case two nodes are equal if they have the same coordinates,
    # accelerations and speeds
    def __eq__(self, other):
        return (self.coord[0] == other.coord[0]) and (self.coord[1] == other.coord[1]) and (self.vel[0] == other.vel[0]) and (self.vel[1] == other.vel[1])

    # Returns the hash of a node
    def __hash__(self):
        return hash(str(self.coord) + str(self.vel))
