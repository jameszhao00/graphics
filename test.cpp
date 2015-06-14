//
// Created by James Zhao on 6/13/15.
//

#include "mesh.capnp.h"

#include "gtest/gtest.h"
#include "content_converter.h"

TEST(ContentImporterTest, CorrectlyImportsTestData) {
    convert_scene("test-data.fbx", "test-data.meshoutput.testoutput");
}