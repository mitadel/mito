// radius of the disk
R = 1.0;
// mesh size
h = 0.1;

Point(0) = { 0, 0, 0, 1.0 };
Point(1) = { R, 0, 0, h };
Point(2) = { 0, R, 0, h };
Point(3) = { -R, 0, 0, h };
Point(4) = { 0, -R, 0, h };

Circle(1) = {2, 0, 3};
Circle(2) = {3, 0, 4};
Circle(3) = {4, 0, 1};
Circle(4) = {1, 0, 2};
Curve Loop(1) = {1, 2, 3, 4};
Plane Surface(1) = {1};
Physical Surface(1) = {1};

Mesh.MshFileVersion = 2.0;