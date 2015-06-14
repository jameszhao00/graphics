//
// Created by James Zhao on 6/13/15.
//

#include <sys/fcntl.h>
#include "../mesh.capnp.h"

#include "gtest/gtest.h"
#include "../content_converter.h"

#include <capnp/message.h>
#include <capnp/serialize-packed.h>

TEST(ContentImporterTest, ImportCrytekSponza) {
    convert_scene("test/testdata/crytek-sponza/sponza.obj", "crytek-sponza.meshoutput.testoutput");
}
TEST(ContentImporterTest, ImportTextureTest) {
    convert_scene("test/testdata/head/head.obj", "head.meshoutput.testoutput");
    int fd = open("head.meshoutput.testoutput", O_RDONLY);
    ::capnp::PackedFdMessageReader message(fd);
    auto mesh_group = message.getRoot<MeshGroup>();

    ASSERT_EQ(2, mesh_group.getMaterials().size());
    ASSERT_TRUE(mesh_group.getMaterials()[1].hasDiffuse());
    ASSERT_EQ(50331648, mesh_group.getMaterials()[1].getDiffuse().size());
}

TEST(ContentImporterTest, ImportGeometryTest) {
    convert_scene("test/testdata/test-data.dae", "test-data.meshoutput.testoutput");
    int fd = open("test-data.meshoutput.testoutput", O_RDONLY);
    ::capnp::PackedFdMessageReader message(fd);
    auto mesh_group = message.getRoot<MeshGroup>();
    ASSERT_EQ(2, mesh_group.getMeshes().size());
    ASSERT_EQ(2, mesh_group.getMaterials().size());
    auto one_triangle_mesh = mesh_group.getMeshes()[1];
    {
        auto vertices = one_triangle_mesh.getGeometry().getVertices();
        ASSERT_EQ(3, vertices.size());
        {
            ASSERT_FLOAT_EQ(.75, vertices[0].getPosition().getX());
            ASSERT_FLOAT_EQ(1.05, vertices[0].getPosition().getY());
            // -1.35 instead of 1.35 because we convert to a left handed CS.
            ASSERT_FLOAT_EQ(-1.35, vertices[0].getPosition().getZ());
        }
        {
            ASSERT_FLOAT_EQ(.25, vertices[1].getPosition().getX());
            ASSERT_FLOAT_EQ(1.05, vertices[1].getPosition().getY());
            // -1.35 instead of 1.35 because we convert to a left handed CS.
            ASSERT_FLOAT_EQ(-1.35, vertices[1].getPosition().getZ());
        }
        {
            ASSERT_FLOAT_EQ(.75, vertices[2].getPosition().getX());
            ASSERT_FLOAT_EQ(1.05, vertices[2].getPosition().getY());
            // -1.65 instead of 1.65 because we convert to a left handed CS.
            ASSERT_FLOAT_EQ(-1.65, vertices[2].getPosition().getZ());
        }
        auto material_index = one_triangle_mesh.getMaterialIndex();
        auto material = mesh_group.getMaterials()[material_index];
        auto albedo = material.getAlbedo();
        ASSERT_FLOAT_EQ(0, albedo.getR());
        ASSERT_FLOAT_EQ(.8, albedo.getG()); // The color is .8 for some reason in the dae file.
        ASSERT_FLOAT_EQ(0, albedo.getB());
    }
}