#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(MeshWriter, PointCloudToVtk)
{
    // an empty point cloud
    auto & cloud = mito::geometry::point_cloud<3>();

    // add points to the point cloud
    const auto point_a = cloud.point({ 0.0, 0.0, 0.0 });
    const auto point_b = cloud.point({ 1.0, 1.0, 0.0 });
    const auto point_c = cloud.point({ 0.0, 1.0, 1.0 });

    // print the point cloud
    mito::io::vtk::writer("point_cloud_output", cloud);
}
