#pragma once

#include <base.hpp>
#include <core/texture.hpp>
#include <glm/glm.hpp>

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <cstdint>

namespace RESTART
{

class Shape
{
public:
    Shape(const std::string& name) : m_name(name) {};

    virtual ~Shape();

protected:
    virtual void generate() = 0;

    std::string m_name;
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
};

class GeometryGenerator
{
public:
    using uint32 = std::uint32_t;

    struct Vertex
    {
        Vertex(){}
        Vertex(
                const glm::vec3& p,
                const glm::vec3& n,
                const glm::vec3& t,
                const glm::vec2& uv) :
                position(p),
                normal(n),
                tangentU(t),
                texCoord(uv){}
        Vertex(
                float px, float py, float pz,
                float nx, float ny, float nz,
                float tx, float ty, float tz,
                float u, float v) :
                position(px,py,pz),
                normal(nx,ny,nz),
                tangentU(tx, ty, tz),
                texCoord(u,v){}

        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangentU;
        glm::vec2 texCoord;
    };

    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<uint32> indices;
    };

    MeshData CreateBox(float width, float height, float depth, uint32 numSubdivisions);

    MeshData CreateSphere(float radius, uint32 sliceCount, uint32 stackCount);

    MeshData CreateGeosphere(float radius, uint32 numSubdivisions);

    MeshData CreateCylinder(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount);

    MeshData CreateGrid(float width, float depth, uint32 m, uint32 n);

    MeshData CreateQuad(float x, float y, float w, float h, float depth);

private:
    void subdivide(MeshData& meshData);
    Vertex midPoint(const Vertex& v0, const Vertex& v1);
    void buildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
    void buildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
};

struct SubmeshGeometry
{
    std::uint32_t indexCount = 0;
    std::uint32_t startIndexLocation = 0;
    std::int32_t baseVertexLocation = 0;
};

struct MeshGeometry
{
    std::string name;

    std::uint32_t vertexByteStride = 0;
    std::uint32_t vertexBufferByteSize = 0;
    std::uint32_t indexBufferByteSize = 0;

    std::unordered_map<std::string, SubmeshGeometry> drawArgs;
};

struct RenderItem
{
    RenderItem() = default;

    glm::mat4 world = MathHelp::identityMat4();

    MeshGeometry* geo = nullptr;

    std::uint32_t indexCount = 0;
    std::uint32_t startIndexLocation = 0;
    std::int32_t baseVertexLocation = 0;

    GLuint vao, ebo;
};

}