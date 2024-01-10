// radius of the disk
R = 0.5;
SetFactory("OpenCASCADE");
// the disk
Circle(1) = {0, 0, 0, R, 0, 2*Pi};
// the disk boundary
Curve Loop(1) = {1};
// the disk surface
Plane Surface(1) = {1};

Mesh.MshFileVersion = 2.0;