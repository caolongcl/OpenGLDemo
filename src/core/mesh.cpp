#include <base.hpp>
#include <core/mesh.hpp>

using namespace std;
using namespace RESTART;

Mesh::Mesh(const string& filename)
        : Mesh()
{
    Assimp::Importer loader;
    const aiScene *scene = loader.ReadFile(PROJECT_RESOURCE_DIR"/model/" + filename,
                                           aiProcessPreset_TargetRealtime_MaxQuality |
                                           aiProcess_OptimizeGraph |
                                           aiProcess_FlipUVs);

    auto index = filename.find_last_of('/');

    if (scene == nullptr)
    {
        fprintf(stderr, "%s\n", loader.GetErrorString());
    }
    else
    {
        parse(filename.substr(0, index), scene->mRootNode, scene);
    }
}

Mesh::Mesh(const vector<Vertex>& vertices,
           const vector<GLuint>& indices,
           const map<GLuint, string>& textures)
        : m_indices(indices), m_vertices(vertices), m_textures(textures)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices.front(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Mesh::draw(GLuint shader)
{
    uint32_t unit = 0, diffuse = 0, specular = 0;

    for (auto& i : m_subMeshes)
    {
        i->draw(shader);
    }

    for (auto& i : m_textures)
    {
        string uniform = i.second;

        if (i.second == "diffuse")
        {
            uniform += (diffuse++ > 0) ? to_string(diffuse) : "";
        }
        else if (i.second == "specular")
        {
            uniform += (specular++ > 0) ? to_string(specular) : "";
        }

        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, i.first);
        glUniform1f(glGetUniformLocation(shader, uniform.c_str()), ++unit);
    }

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
}

void Mesh::parse(const string& path, const aiNode *node, const aiScene *scene)
{
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        parse(path, scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        parse(path, node->mChildren[i], scene);
    }
}

void Mesh::parse(const string& path, const aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex> vertices;
    Vertex vertex;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        if (mesh->mTextureCoords[0])
        {
            vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertices.push_back(vertex);
    }

    vector<uint32_t> indices;

    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        for (uint32_t j = 0; j < mesh->mFaces[i].mNumIndices; j++)
        {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    std::map<GLuint, std::string> textures;
    auto diffuse = process(path, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);
    auto specular = process(path, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR);
    textures.insert(diffuse.begin(), diffuse.end());
    textures.insert(specular.begin(), specular.end());

    //m_subMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(vertices, indices, textures)));
    m_subMeshes.push_back(make_unique<Mesh>(vertices, indices, textures));
}

map<GLuint, string> Mesh::process(const string& path, aiMaterial *material, aiTextureType type)
{
    map<GLuint, std::string> textures;

    for (uint32_t i = 0; i < material->GetTextureCount(type); i++)
    {
        GLenum format = GL_RGB;
        GLuint texture;
        string mode;

        aiString str;
        material->GetTexture(type, i, &str);

        string filename = str.C_Str();
        filename = PROJECT_RESOURCE_DIR"/model/" + path + "/" + filename;

        int width, height, channels;
        unsigned char *image = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (image == nullptr)
        {
            fprintf(stderr, "%s %s\n", "Failed to Load Texture", filename.c_str());
        }

        switch (channels)
        {
        case 1 :
            format = GL_ALPHA;
            break;
        case 2 :
            format = GL_ALPHA; /*GL_LUMINANCE;*/ break;
        case 3 :
            format = GL_RGB;
            break;
        case 4 :
            format = GL_RGBA;
            break;
        default:
            break;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);

        if (type == aiTextureType_DIFFUSE)
        {
            mode = "diffuse";
        }
        else if (type == aiTextureType_SPECULAR)
        {
            mode = "specular";
        }

        textures.insert(make_pair(texture, mode));
    }

    return textures;
}
