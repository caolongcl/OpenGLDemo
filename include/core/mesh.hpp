#pragma once

#include <base.hpp>

#include <map>
#include <memory>
#include <vector>
#include <string>

namespace RESTART
{
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh
{
public:
    Mesh() { glGenVertexArrays(1, &m_vao); }

    ~Mesh() { glDeleteVertexArrays(1, &m_vao); }

    Mesh(const std::string& filename);

    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<std::uint32_t>& indices,
         const std::map<GLuint, std::string>& textures);

    void draw(GLuint shader);

private:
    Mesh(const Mesh&) = delete;

    Mesh& operator=(const Mesh&) = delete;

    void parse(const std::string& path, const aiNode *node, const aiScene *scene);

    void parse(const std::string& path, const aiMesh *mesh, const aiScene *scene);

    std::map<GLuint, std::string> process(const std::string& path, aiMaterial *material, aiTextureType type);

    std::vector<std::unique_ptr<Mesh>> m_subMeshes;
    std::vector<std::uint32_t> m_indices;
    std::vector<Vertex> m_vertices;
    std::map<GLuint, std::string> m_textures;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
};
};
