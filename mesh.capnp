@0xce42159e15507b5b;

struct Vector3 {
    x @0 : Float32;
    y @1 : Float32;
    z @2 : Float32;
}
struct Color {
    r @0 : Float32;
    g @1 : Float32;
    b @2 : Float32;
}
struct Vertex {
    position @0 : Vector3;
}
struct Geometry {
    vertices @0 : List(Vertex);
    indices @1 : List(Int32);
}
struct Material {
    albedo @0 : Color;
    diffuse @1 : Data;
}
struct Mesh {
    geometry @0 : Geometry;
    materialIndex @1 : Int32;
}
# unused
struct MeshInstance {
    position @0 : Vector3;
    meshIndex @1 : Int32;
}
# end unused
struct MeshGroup {
    meshes @0 : List(Mesh);
    #meshInstances @1 : List(MeshInstance);
    materials @1 : List(Material);
}