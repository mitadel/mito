#
# Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
#

import math

# convert cartesian coordinates to polar coordinates
def cartesian_to_polar(x, y):
    r = math.sqrt(x**2 + y**2)
    theta = math.atan2(y, x)
    return r, theta

def polar_to_cartesian(r, theta):
    x = r * math.cos(theta)
    y = r * math.sin(theta)
    return x, y

# open input and output files
input_file_path = 'disk_cartesian.summit'
input_file = open(input_file_path, 'r')
output_file_path = 'disk_polar.summit'
output_file = open(output_file_path, 'w')

# copy heading of input file straight to output file
dim = input_file.readline().strip()
output_file.write(f"{dim}\n")
n_points, n_elements, n_materials = map(int, input_file.readline().split())
output_file.write(f"{n_points} {n_elements} {n_materials}\n")

# loop on the points of the input file
for _ in range(n_points):
    # read the points in cartesian coordinates
    x, y = map(float, input_file.readline().split())
    # convert them to polar coordinates
    r, theta = cartesian_to_polar(x, y)
    # write the converted points to the output file
    output_file.write(f"{r} {theta}\n")

# copy the element connectivity straight to output file
for _ in range(n_elements):
    output_file.write(input_file.readline())