//
// Created by James Zhao on 6/13/15.
//

#include "content_converter.h"

#include "assimp/importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <glog/logging.h>

#include "mesh.capnp.h"


#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <sys/fcntl.h>

void foo() {

}

void to_color(aiColor3D color, Color::Builder color_output) {
    color_output.setR(color.r);
    color_output.setG(color.g);
    color_output.setB(color.b);
}

void convert_vertex(aiVector3D vertex, Vector3::Builder position_output) {
    position_output.setX(vertex.x);
    position_output.setY(vertex.y);
    position_output.setZ(vertex.z);
}

void convert_vertices(const aiMesh *mesh, capnp::List<Vertex>::Builder vertices_output) {
    for (auto vertex_index = 0; vertex_index < mesh->mNumVertices; vertex_index++) {
        auto vertex = mesh->mVertices[vertex_index];
        auto vertex_output = vertices_output[vertex_index];
        auto position_output = vertex_output.initPosition();
        convert_vertex(vertex, position_output);
    }
}

void convert_indices(const aiMesh *mesh, capnp::List<int32_t>::Builder &indices_output) {
    for (auto face_index = 0, indice_index = 0;
         face_index < mesh->mNumFaces;
         face_index++, indice_index += 3) {
        aiFace face = mesh->mFaces[face_index];
        CHECK_EQ(face.mNumIndices, 3) << "Only triangular faces are supported. Received face with " <<
                                      face.mNumIndices << " verties";
        for (int face_vertices_index = 0; face_vertices_index < 3; face_vertices_index++) {
            indices_output.set(indice_index + face_vertices_index, face.mIndices[face_vertices_index]);
        }
    }
}

void convert_mesh(const aiMesh *mesh, Geometry::Builder geometry_output) {
    LOG(INFO)
    << "Converting mesh with "
    << mesh->mNumVertices
    << " vertices and "
    << mesh->mNumFaces
    << " triangles.";

    auto vertices_output = geometry_output.initVertices(mesh->mNumVertices);
    convert_vertices(mesh, vertices_output);
    CHECK_EQ(mesh->mPrimitiveTypes, aiPrimitiveType_TRIANGLE);
    auto indices_output = geometry_output.initIndices(mesh->mNumFaces * 3);
    convert_indices(mesh, indices_output);
}

void convert_material(const aiMaterial *material, Material::Builder material_output) {
    aiColor3D albedo;
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, albedo) == AI_SUCCESS) {
        LOG(INFO) << "Material has albedo.";
        to_color(albedo, material_output.initAlbedo());
    }
}

void convert_materials(const aiScene *scene, capnp::List<Material>::Builder &materials_output) {
    for (auto material_index = 0; material_index < scene->mNumMaterials; material_index++) {
        LOG(INFO) << "Importing material " << material_index;
        auto material_output = materials_output[material_index];
        aiMaterial *material = scene->mMaterials[material_index];
        convert_material(material, material_output);
    }
}

void convert_scene(std::string input_path, std::string output_path) {
    LOG(INFO) << "Converting scene...";
    Assimp::Importer importer;
    auto *scene = importer.ReadFile(input_path, aiProcess_CalcTangentSpace
                                                | aiProcess_Triangulate
                                                | aiProcess_JoinIdenticalVertices
                                                | aiProcess_MakeLeftHanded
                                                | aiProcess_PreTransformVertices);
    CHECK(scene != nullptr) << "Import failed : " << importer.GetErrorString();
    ::capnp::MallocMessageBuilder message;
    auto mesh_group = message.initRoot<MeshGroup>();

    auto meshes_output = mesh_group.initMeshes(scene->mNumMeshes);
    auto materials_output = mesh_group.initMaterials(scene->mNumMaterials);

    convert_materials(scene, materials_output);
    LOG(INFO) << "Found " << scene->mNumMeshes << " mesh(es)";
    for (auto mesh_index = 0; mesh_index < scene->mNumMeshes; mesh_index++) {
        LOG(INFO) << "Found mesh with index " << mesh_index;
        auto mesh_output = meshes_output[mesh_index];
        auto geometry_output = mesh_output.initGeometry();
        auto *mesh = scene->mMeshes[mesh_index];
        convert_mesh(mesh, geometry_output);
        mesh_output.setMaterialIndex(mesh->mMaterialIndex);
    }
    int fd = open(output_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    CHECK_GE(fd, -1) << "Couldn't open file for write: Error number: " << errno;
    writePackedMessageToFd(fd, message);
    close(fd);
}