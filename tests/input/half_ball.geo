cl__1 = 1;
r = 1;
h = 0.5;
Point(1) = {0, 0, 0, h};
Point(2) = {r, 0, 0, h};
Point(3) = {-r, 0, 0, h};
Point(4) = {0, r, 0, h};
Point(5) = {0, -r, 0, h};
Point(6) = {0, 0, r, h};
Point(7) = {0, 0, -r, h};
Circle(1) = {2, 1, 4};
Circle(2) = {4, 1, 3};
Circle(3) = {3, 1, 5};
Circle(4) = {5, 1, 2};
Circle(5) = {2, 1, 6};
Circle(6) = {6, 1, 3};
Circle(9) = {4, 1, 6};
Circle(10) = {6, 1, 5};
Line Loop(16) = {2, -6, -9};
Ruled Surface(16) = {16};
Line Loop(18) = {3, -10, 6};
Ruled Surface(18) = {18};
Line Loop(24) = {4, 5, 10};
Ruled Surface(24) = {24};
Line Loop(26) = {1, 9, -5};
Ruled Surface(26) = {26};
Line Loop(29) = {4, 1, 2, 3};
Surface(29) = {29};
Surface Loop(30) = {16, 18, 29, 24, 26};
Volume(30) = {30};
Physical Volume(1) = {30};