// hollow_half_unit_disk.geo
// Semi-annulus: y >= 0, inner radius r, outer radius R

SetFactory("OpenCASCADE");

// inner radius
r = 0.25;
// outer radius (unit)
R   = 1.0;

// --- Geometry (2D) ---
Point(1) = {0, 0, 0};          // center
Point(2) = { R, 0, 0};         // outer right
Point(3) = {-R, 0, 0};         // outer left
Point(4) = { r, 0, 0};         // inner right
Point(5) = {-r, 0, 0};         // inner left

// Arcs (upper half)
Circle(1) = {3, 1, 2};         // outer arc from (R,0) to (-R,0) via center
Circle(2) = {5, 1, 4};         // inner arc from (-r,0) to (r,0) via center

// Diameter segments
Line(3) = {4, 2};              // right segment: inner->outer
Line(4) = {3, 5};              // left segment: outer->inner

// Curve loop (orientation matters)
Curve Loop(10) = {3, 2, 4, 1};
// {1, 4, 2, 3};
Plane Surface(11) = {10};

// --- Physical groups (useful for BCs) ---
Physical Surface("domain") = {11};

Physical Curve("outer_arc") = {1};
Physical Curve("inner_arc") = {2};
Physical Curve("diameter_right") = {3};
Physical Curve("diameter_left")  = {4};

// --- Mesh settings (unstructured) ---
Mesh.Algorithm = 6;            // Frontal-Delaunay (often nice); try 5 (Delaunay) too
Mesh.CharacteristicLengthMin = 0.02;
Mesh.CharacteristicLengthMax = 0.06;
