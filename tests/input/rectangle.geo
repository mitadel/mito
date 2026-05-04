H = 0.05;
L = 0.025;
h = 0.0009;

Point(1) = {0.0,0.0,0.0,h};
Point(2) = {H,0.0,0.0,h};
Point(3) = {H,L,0.0,h};
Point(4) = {0,L,0.0,h};
Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};
Line Loop(5) = {1,2,3,4};
Physical Line ("1") = {4};
Plane Surface(1) = {5};
Physical Surface("rectangle") = {1};

