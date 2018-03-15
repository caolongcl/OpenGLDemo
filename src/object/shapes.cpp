#include <object/shapes.hpp>

using namespace std;
using namespace RESTART;

GeometryGenerator::MeshData GeometryGenerator::CreateBox(float width, float height, float depth, uint32 numSubdivisions)
{
    MeshData meshData;

    Vertex v[24];

    float w2 = 0.5f * width;
    float h2 = 0.5f * height;
    float d2 = 0.5f * depth;

    // Fill in the front face vertex data.
    v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // Fill in the back face vertex data.
    v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Fill in the top face vertex data.
    v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // Fill in the bottom face vertex data.
    v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Fill in the left face vertex data.
    v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
    v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

    // Fill in the right face vertex data.
    v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    meshData.vertices.assign(begin(v), end(v));

    //
    // Create the indices.
    //

    uint32 index[36];

    // Fill in the front face index data
    index[0] = 0;
    index[1] = 1;
    index[2] = 2;
    index[3] = 0;
    index[4] = 2;
    index[5] = 3;

    // Fill in the back face index data
    index[6] = 4;
    index[7] = 5;
    index[8] = 6;
    index[9] = 4;
    index[10] = 6;
    index[11] = 7;

    // Fill in the top face index data
    index[12] = 8;
    index[13] = 9;
    index[14] = 10;
    index[15] = 8;
    index[16] = 10;
    index[17] = 11;

    // Fill in the bottom face index data
    index[18] = 12;
    index[19] = 13;
    index[20] = 14;
    index[21] = 12;
    index[22] = 14;
    index[23] = 15;

    // Fill in the left face index data
    index[24] = 16;
    index[25] = 17;
    index[26] = 18;
    index[27] = 16;
    index[28] = 18;
    index[29] = 19;

    // Fill in the right face index data
    index[30] = 20;
    index[31] = 21;
    index[32] = 22;
    index[33] = 20;
    index[34] = 22;
    index[35] = 23;

    meshData.indices.assign(begin(index), end(index));

    // Put a cap on the number of subdivisions.
    numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

    for (uint32 i = 0; i < numSubdivisions; ++i)
    {
        subdivide(meshData);
    }

    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateSphere(float radius, uint32 sliceCount, uint32 stackCount)
{
    MeshData meshData{};

    //
    // Compute the vertices stating at the top pole and moving down the stacks.
    //

    // Poles: note that there will be texture coordinate distortion as there is
    // not a unique point on the texture map to assign to the pole when mapping
    // a rectangular texture onto a sphere.
    Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    meshData.vertices.push_back(topVertex);

    auto phiStep = static_cast<float>(M_PI / stackCount);
    auto thetaStep = static_cast<float>(2.0f * M_PI / sliceCount);

    // Compute vertices for each stack ring (do not count the poles as rings).
    for (uint32 i = 1; i <= stackCount - 1; ++i)
    {
        float phi = i * phiStep;

        // vertices of ring.
        for (uint32 j = 0; j <= sliceCount; ++j)
        {
            float theta = j * thetaStep;

            Vertex v;

            // spherical to cartesian
            v.position.x = radius * sinf(phi) * cosf(theta);
            v.position.y = radius * cosf(phi);
            v.position.z = radius * sinf(phi) * sinf(theta);

            // Partial derivative of P with respect to theta
            v.tangentU.x = -radius * sinf(phi) * sinf(theta);
            v.tangentU.y = 0.0f;
            v.tangentU.z = +radius * sinf(phi) * cosf(theta);

            v.tangentU = glm::normalize(v.tangentU);
            v.normal = glm::normalize(v.position);

            v.texCoord.x = static_cast<float>(theta / (2.0f * M_PI));
            v.texCoord.y = static_cast<float>(phi / M_PI);

            meshData.vertices.push_back(v);
        }
    }

    meshData.vertices.push_back(bottomVertex);

    //
    // Compute indices for top stack.  The top stack was written first to the vertex buffer
    // and connects the top pole to the first ring.
    //

    for (uint32 i = 1; i <= sliceCount; ++i)
    {
        meshData.indices.push_back(0);
        meshData.indices.push_back(i + 1);
        meshData.indices.push_back(i);
    }

    //
    // Compute indices for inner stacks (not connected to poles).
    //

    // Offset the indices to the index of the first vertex in the first ring.
    // This is just skipping the top pole vertex.
    uint32 baseIndex = 1;
    uint32 ringVertexCount = sliceCount + 1;
    for (uint32 i = 0; i < stackCount - 2; ++i)
    {
        for (uint32 j = 0; j < sliceCount; ++j)
        {
            meshData.indices.push_back(baseIndex + i * ringVertexCount + j);
            meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

            meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    //
    // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
    // and connects the bottom pole to the bottom ring.
    //

    // South pole vertex was added last.
    uint32 southPoleIndex = (uint32) meshData.vertices.size() - 1;

    // Offset the indices to the index of the first vertex in the last ring.
    baseIndex = southPoleIndex - ringVertexCount;

    for (uint32 i = 0; i < sliceCount; ++i)
    {
        meshData.indices.push_back(southPoleIndex);
        meshData.indices.push_back(baseIndex + i);
        meshData.indices.push_back(baseIndex + i + 1);
    }

    return meshData;
}

void GeometryGenerator::subdivide(MeshData& meshData)
{
    // Save a copy of the input geometry.
    MeshData inputCopy = meshData;


    meshData.vertices.resize(0);
    meshData.indices.resize(0);

    //       v1
    //       *
    //      / \
	//     /   \
	//  m0*-----*m1
    //   / \   / \
	//  /   \ /   \
	// *-----*-----*
    // v0    m2     v2

    uint32 numTris = (uint32) inputCopy.indices.size() / 3;
    for (uint32 i = 0; i < numTris; ++i)
    {
        Vertex v0 = inputCopy.vertices[inputCopy.indices[i * 3 + 0]];
        Vertex v1 = inputCopy.vertices[inputCopy.indices[i * 3 + 1]];
        Vertex v2 = inputCopy.vertices[inputCopy.indices[i * 3 + 2]];

        //
        // Generate the midpoints.
        //

        Vertex m0 = midPoint(v0, v1);
        Vertex m1 = midPoint(v1, v2);
        Vertex m2 = midPoint(v0, v2);

        //
        // Add new geometry.
        //

        meshData.vertices.push_back(v0); // 0
        meshData.vertices.push_back(v1); // 1
        meshData.vertices.push_back(v2); // 2
        meshData.vertices.push_back(m0); // 3
        meshData.vertices.push_back(m1); // 4
        meshData.vertices.push_back(m2); // 5

        meshData.indices.push_back(i * 6 + 0);
        meshData.indices.push_back(i * 6 + 3);
        meshData.indices.push_back(i * 6 + 5);

        meshData.indices.push_back(i * 6 + 3);
        meshData.indices.push_back(i * 6 + 4);
        meshData.indices.push_back(i * 6 + 5);

        meshData.indices.push_back(i * 6 + 5);
        meshData.indices.push_back(i * 6 + 4);
        meshData.indices.push_back(i * 6 + 2);

        meshData.indices.push_back(i * 6 + 3);
        meshData.indices.push_back(i * 6 + 1);
        meshData.indices.push_back(i * 6 + 4);
    }
}

GeometryGenerator::Vertex GeometryGenerator::midPoint(const Vertex& v0, const Vertex& v1)
{
    // Compute the midpoints of all the attributes.  Vectors need to be normalized
    // since linear interpolating can make them not unit length.

    Vertex v;
    v.position = v0.position;
    v.position += v1.position;
    v.position *= 0.5f;

    v.normal = v0.normal;
    v.normal += v1.normal;
    v.normal *= 0.5f;
    v.normal = glm::normalize(v.normal);

    v.tangentU = v0.tangentU;
    v.tangentU += v1.tangentU;
    v.tangentU *= 0.5f;
    v.tangentU = glm::normalize(v.tangentU);

    v.texCoord = v0.texCoord;
    v.texCoord += v1.texCoord;
    v.texCoord *= 0.5f;

    return v;
}

GeometryGenerator::MeshData GeometryGenerator::CreateGeosphere(float radius, uint32 numSubdivisions)
{
    MeshData meshData;

    // Put a cap on the number of subdivisions.
    numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

    // Approximate a sphere by tessellating an icosahedron.

    const float X = 0.525731f;
    const float Z = 0.850651f;

    glm::vec3 pos[12] =
            {
                    glm::vec3(-X, 0.0f, Z), glm::vec3(X, 0.0f, Z),
                    glm::vec3(-X, 0.0f, -Z), glm::vec3(X, 0.0f, -Z),
                    glm::vec3(0.0f, Z, X), glm::vec3(0.0f, Z, -X),
                    glm::vec3(0.0f, -Z, X), glm::vec3(0.0f, -Z, -X),
                    glm::vec3(Z, X, 0.0f), glm::vec3(-Z, X, 0.0f),
                    glm::vec3(Z, -X, 0.0f), glm::vec3(-Z, -X, 0.0f)
            };

    uint32 k[60] =
            {
                    1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4,
                    1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2,
                    3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0,
                    10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
            };

    meshData.vertices.resize(12);
    meshData.indices.assign(begin(k), end(k));

    for (uint32 i = 0; i < 12; ++i)
    {
        meshData.vertices[i].position = pos[i];
    }

    for (uint32 i = 0; i < numSubdivisions; ++i)
    {
        subdivide(meshData);
    }

    // Project vertices onto sphere and scale.
    for (auto& vertex : meshData.vertices)
    {
        // Project onto unit sphere.
        glm::vec3 normal = glm::normalize(vertex.position);

        // Project onto sphere.
        vertex.normal = glm::normalize(vertex.position);
        vertex.position = vertex.normal;
        vertex.position *= radius;

        // Derive texture coordinates from spherical coordinates.
        float theta = atan2f(vertex.position.z, vertex.position.x);

        // Put in [0, 2pi].
        if (theta < 0.0f)
            theta += 2.0f * M_PI;

        float phi = acosf(vertex.position.y / radius);

        vertex.texCoord.x = theta / (2.0f * M_PI);
        vertex.texCoord.y = phi / M_PI;

        // Partial derivative of P with respect to theta
        vertex.tangentU.x = -radius * sinf(phi) * sinf(theta);
        vertex.tangentU.y = 0.0f;
        vertex.tangentU.z = +radius * sinf(phi) * cosf(theta);
        vertex.tangentU = glm::normalize(vertex.tangentU);
    }


    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount)
{
    MeshData meshData{};

    //
    // Build Stacks.
    // 

    float stackHeight = height / stackCount;

    // Amount to increment radius as we move up each stack level from bottom to top.
    float radiusStep = (topRadius - bottomRadius) / stackCount;

    uint32 ringCount = stackCount + 1;

    // Compute vertices for each stack ring starting at the bottom and moving up.
    for (uint32 i = 0; i < ringCount; ++i)
    {
        float y = -0.5f * height + i * stackHeight;
        float r = bottomRadius + i * radiusStep;

        // vertices of ring
        float dTheta = 2.0f * M_PI / sliceCount;
        for (uint32 j = 0; j <= sliceCount; ++j)
        {
            Vertex vertex;

            float c = cosf(j * dTheta);
            float s = sinf(j * dTheta);

            vertex.position = glm::vec3(r * c, y, r * s);

            vertex.texCoord.x = (float) j / sliceCount;
            vertex.texCoord.y = 1.0f - (float) i / stackCount;

            // Cylinder can be parameterized as follows, where we introduce v
            // parameter that goes in the same direction as the v tex-coord
            // so that the bitangent goes in the same direction as the v tex-coord.
            //   Let r0 be the bottom radius and let r1 be the top radius.
            //   y(v) = h - hv for v in [0,1].
            //   r(v) = r1 + (r0-r1)v
            //
            //   x(t, v) = r(v)*cos(t)
            //   y(t, v) = h - hv
            //   z(t, v) = r(v)*sin(t)
            // 
            //  dx/dt = -r(v)*sin(t)
            //  dy/dt = 0
            //  dz/dt = +r(v)*cos(t)
            //
            //  dx/dv = (r0-r1)*cos(t)
            //  dy/dv = -h
            //  dz/dv = (r0-r1)*sin(t)

            // This is unit length.
            vertex.tangentU = glm::vec3(-s, 0.0f, c);

            float dr = bottomRadius - topRadius;
            glm::vec3 bitangent(dr * c, -height, dr * s);

            vertex.normal = glm::normalize(glm::cross(vertex.tangentU, bitangent));

            meshData.vertices.push_back(vertex);
        }
    }

    // Add one because we duplicate the first and last vertex per ring
    // since the texture coordinates are different.
    uint32 ringVertexCount = sliceCount + 1;

    // Compute indices for each stack.
    for (uint32 i = 0; i < stackCount; ++i)
    {
        for (uint32 j = 0; j < sliceCount; ++j)
        {
            meshData.indices.push_back(i * ringVertexCount + j);
            meshData.indices.push_back((i + 1) * ringVertexCount + j);
            meshData.indices.push_back((i + 1) * ringVertexCount + j + 1);

            meshData.indices.push_back(i * ringVertexCount + j);
            meshData.indices.push_back((i + 1) * ringVertexCount + j + 1);
            meshData.indices.push_back(i * ringVertexCount + j + 1);
        }
    }

    buildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
    buildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);

    return meshData;
}

void GeometryGenerator::buildCylinderTopCap(float bottomRadius, float topRadius, float height,
                                            uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
    uint32 baseIndex = (uint32) meshData.vertices.size();

    float y = 0.5f * height;
    float dTheta = 2.0f * M_PI / sliceCount;

    // Duplicate cap ring vertices because the texture coordinates and normals differ.
    for (uint32 i = 0; i <= sliceCount; ++i)
    {
        float x = topRadius * cosf(i * dTheta);
        float z = topRadius * sinf(i * dTheta);

        // Scale down by the height to try and make top cap texture coord area
        // proportional to base.
        float u = x / height + 0.5f;
        float v = z / height + 0.5f;

        meshData.vertices.emplace_back(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v);
    }

    // Cap center vertex.
    meshData.vertices.emplace_back(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f);

    // Index of center vertex.
    uint32 centerIndex = (uint32) meshData.vertices.size() - 1;

    for (uint32 i = 0; i < sliceCount; ++i)
    {
        meshData.indices.push_back(centerIndex);
        meshData.indices.push_back(baseIndex + i + 1);
        meshData.indices.push_back(baseIndex + i);
    }
}

void GeometryGenerator::buildCylinderBottomCap(float bottomRadius, float topRadius, float height,
                                               uint32 sliceCount, uint32 stackCount, MeshData& meshData)
{
    // 
    // Build bottom cap.
    //

    uint32 baseIndex = (uint32) meshData.vertices.size();
    float y = -0.5f * height;

    // vertices of ring
    float dTheta = 2.0f * M_PI / sliceCount;
    for (uint32 i = 0; i <= sliceCount; ++i)
    {
        float x = bottomRadius * cosf(i * dTheta);
        float z = bottomRadius * sinf(i * dTheta);

        // Scale down by the height to try and make top cap texture coord area
        // proportional to base.
        float u = x / height + 0.5f;
        float v = z / height + 0.5f;

        meshData.vertices.emplace_back(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v);
    }

    // Cap center vertex.
    meshData.vertices.emplace_back(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f);

    // Cache the index of center vertex.
    uint32 centerIndex = (uint32) meshData.vertices.size() - 1;

    for (uint32 i = 0; i < sliceCount; ++i)
    {
        meshData.indices.push_back(centerIndex);
        meshData.indices.push_back(baseIndex + i);
        meshData.indices.push_back(baseIndex + i + 1);
    }
}

GeometryGenerator::MeshData GeometryGenerator::CreateGrid(float width, float depth, uint32 m, uint32 n)
{
    MeshData meshData{};

    uint32 vertexCount = m * n;
    uint32 faceCount = (m - 1) * (n - 1) * 2;

    //
    // Create the vertices.
    //

    float halfWidth = 0.5f * width;
    float halfDepth = 0.5f * depth;

    float dx = width / (n - 1);
    float dz = depth / (m - 1);

    float du = 1.0f / (n - 1);
    float dv = 1.0f / (m - 1);

    meshData.vertices.resize(vertexCount);
    for (uint32 i = 0; i < m; ++i)
    {
        float z = halfDepth - i * dz;
        for (uint32 j = 0; j < n; ++j)
        {
            float x = -halfWidth + j * dx;

            meshData.vertices[i * n + j].position = glm::vec3(x, 0.0f, z);
            meshData.vertices[i * n + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);
            meshData.vertices[i * n + j].tangentU = glm::vec3(1.0f, 0.0f, 0.0f);

            // Stretch texture over grid.
            meshData.vertices[i * n + j].texCoord.x = j * du;
            meshData.vertices[i * n + j].texCoord.y = i * dv;
        }
    }

    //
    // Create the indices.
    //

    meshData.indices.resize(faceCount * 3); // 3 indices per face

    // Iterate over each quad and compute indices.
    uint32 k = 0;
    for (uint32 i = 0; i < m - 1; ++i)
    {
        for (uint32 j = 0; j < n - 1; ++j)
        {
            meshData.indices[k] = i * n + j;
            meshData.indices[k + 1] = i * n + j + 1;
            meshData.indices[k + 2] = (i + 1) * n + j;

            meshData.indices[k + 3] = (i + 1) * n + j;
            meshData.indices[k + 4] = i * n + j + 1;
            meshData.indices[k + 5] = (i + 1) * n + j + 1;

            k += 6; // next quad
        }
    }

    return meshData;
}

GeometryGenerator::MeshData GeometryGenerator::CreateQuad(float x, float y, float w, float h, float depth)
{
    MeshData meshData{};

    meshData.vertices.resize(4);
    meshData.indices.resize(6);

    // position coordinates specified in NDC space.
    meshData.vertices[0] = Vertex(
            x, y - h, depth,
            0.0f, 0.0f, -1.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f);

    meshData.vertices[1] = Vertex(
            x, y, depth,
            0.0f, 0.0f, -1.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 0.0f);

    meshData.vertices[2] = Vertex(
            x + w, y, depth,
            0.0f, 0.0f, -1.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f);

    meshData.vertices[3] = Vertex(
            x + w, y - h, depth,
            0.0f, 0.0f, -1.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 1.0f);

    meshData.indices[0] = 0;
    meshData.indices[1] = 1;
    meshData.indices[2] = 2;

    meshData.indices[3] = 0;
    meshData.indices[4] = 2;
    meshData.indices[5] = 3;

    return meshData;
}
