//
// Created by James Zhao on 6/13/15.
//

#include <sys/fcntl.h>
#include "mesh.capnp.h"

#include "gtest/gtest.h"
#include "content_converter.h"

#include <capnp/message.h>
#include <capnp/serialize-packed.h>

TEST(ContentImporterTest, CorrectlyImportsTestData) {
    convert_scene("test-data.fbx", "test-data.meshoutput.testoutput");
    int fd = open("test-data.meshoutput.testoutput", O_RDONLY);
    ::capnp::PackedFdMessageReader message(fd);
    auto mesh_group = message.getRoot<MeshGroup>();
    ASSERT_EQ(2, mesh_group.getMeshes().size());
}