#define GLM_ENABLE_EXPERIMENTAL
#define GL_GLEXT_PROTOTYPES 1

#include <memory>
#include <algorithm>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>

#include "GraphicalObject2d.h"
#include "Vector2d.h"
#include "Debug.h"
#include "Exception.h"
#include "ShaderProcessor.h"

namespace ample::graphics
{
static std::vector<Vector3d<float>> generateFaceNormals(const std::vector<Vector3d<float>> &vert)
{
    return {vert.size(), {0, 0, -1}};
}

static std::vector<Vector3d<float>> generateSideNormals(const std::vector<Vector3d<float>> &vert, normalsMode mode = normalsMode::FACE)
{
    ASSERT(vert.size() % 3 == 0);
    size_t verts = vert.size();
    std::vector<Vector3d<float>> normals;
    std::vector<glm::vec3> triangleNormals;
    std::vector<int> normId;
    for (size_t i = 0; i < verts; i += 3)
    {
        glm::vec3 first{vert[i + 0].x, vert[i + 0].y, vert[i + 0].z};
        glm::vec3 secon{vert[i + 1].x, vert[i + 1].y, vert[i + 1].z};
        glm::vec3 third{vert[i + 2].x, vert[i + 2].y, vert[i + 2].z};
        auto norm = glm::triangleNormal(std::move(first),
                                        std::move(secon),
                                        std::move(third));
        triangleNormals.emplace_back(norm);
        triangleNormals.emplace_back(norm);
        triangleNormals.emplace_back(norm);
        for (int i = 0; i < 3; ++i)
        {
            normals.emplace_back(norm.x, norm.y, norm.z);
        }
    }
    if (mode == normalsMode::VERTEX)
    {
        auto normalsCopy{triangleNormals};
        for (size_t i = 2; i < verts; i += 6)
        {
            size_t f = (i + 2) % verts;
            size_t s = (i + 5) % verts;
            auto mid = (normalsCopy[i] + normalsCopy[f] + normalsCopy[s]) * (1.0f / 3.0f);
            normals[i] = normals[f] = normals[s] = {mid.x, mid.y, mid.z};
        }
        for (size_t i = 5; i < verts; i += 6)
        {
            size_t f = (i + 1) % verts;
            size_t s = (i + 4) % verts;
            auto mid = (normalsCopy[i] + normalsCopy[f] + normalsCopy[s]) * (1.0f / 3.0f);
            normals[i] = normals[f] = normals[s] = {mid.x, mid.y, mid.z};
        }
    }
    return normals;
}

GraphicalObject2d::GraphicalObject2d(const std::vector<Vector2d<float>> &graphicalShape,
                                     const float depth,
                                     const float z,
                                     const bool smooth)
    : _graphicalShape(graphicalShape),
      _depth(depth),
      _z(z)
{
    DEBUG("Setup graphical object 2d");
    exception::OpenGLException::handle();
    std::vector<Vector3d<float>> sideArray;
    std::vector<Vector3d<float>> faceArray;
    size_t verts = graphicalShape.size();
    for (size_t vId = 0; vId < verts; ++vId)
    {
        int nvt = (vId + 1) % verts;
        sideArray.emplace_back(graphicalShape[vId].x, graphicalShape[vId].y, z);
        sideArray.emplace_back(graphicalShape[vId].x, graphicalShape[vId].y, depth + z);
        sideArray.emplace_back(graphicalShape[nvt].x, graphicalShape[nvt].y, depth + z);

        sideArray.emplace_back(graphicalShape[vId].x, graphicalShape[vId].y, z);
        sideArray.emplace_back(graphicalShape[nvt].x, graphicalShape[nvt].y, depth + z);
        sideArray.emplace_back(graphicalShape[nvt].x, graphicalShape[nvt].y, z);
    }
    for (size_t i = 1; i < verts - 1; ++i)
    {
        faceArray.emplace_back(graphicalShape[0].x, graphicalShape[0].y, z);
        faceArray.emplace_back(graphicalShape[i].x, graphicalShape[i].y, z);
        faceArray.emplace_back(graphicalShape[i + 1].x, graphicalShape[i + 1].y, z);
    }

    normalsMode mode = smooth ? normalsMode::VERTEX : normalsMode::FACE;

    _sideArray = std::make_unique<VertexArray>(sideArray, normalsMode::FACE, generateSideNormals(sideArray, mode));
    _faceArray = std::make_unique<VertexArray>(faceArray, normalsMode::SINGLE, generateFaceNormals(faceArray));
    DEBUG("Setup graphical object 2d done!");
}

void GraphicalObject2d::drawSelf()
{
    _sideArray->execute();
    _faceArray->execute();
    exception::OpenGLException::handle();
}
} // namespace ample::graphics
