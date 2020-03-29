#define GLM_ENABLE_EXPERIMENTAL
#define GL_GLEXT_PROTOTYPES 1

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GraphicalObject.h"
#include "ShaderProcessor.h"
#include "Exception.h"
#include "Debug.h"

namespace ample::graphics
{
GraphicalObject::GraphicalObject(const glm::mat4 &translated = glm::mat4{1.0f},
                                 const glm::mat4 &scaled = glm::mat4{1.0f},
                                 const glm::mat4 &rotated = glm::mat4{1.0f})
    : _translated(translated),
      _scaled(scaled),
      _rotated(rotated),
      _modelMatrixUniform(_modelMatrix, "model_matrix") {}

void GraphicalObject::addSubObject(GraphicalObject &object)
{
    activity::Behaviour::addBehaviour(object);
    _subObjects.push_back(&object);
}

float GraphicalObject::getX() const { return _translated[0][0]; }
float GraphicalObject::getY() const { return _translated[1][1]; }
float GraphicalObject::getZ() const { return _translated[2][2]; }

void GraphicalObject::setTranslate(const glm::vec3 &vector) noexcept
{
    _translated = glm::translate(vector);
}
void GraphicalObject::translate(const glm::vec3 &vector) noexcept
{
    _translated *= glm::translate(vector);
}

void GraphicalObject::setRotate(const glm::vec3 &axis, const float angle) noexcept
{
    _rotated = glm::rotate(glm::radians(angle), axis);
}
void GraphicalObject::rotate(const glm::vec3 &axis, const float angle) noexcept
{
    _rotated = glm::rotate(_rotated, glm::radians(angle), axis);
}

void GraphicalObject::setScale(const glm::vec3 &coef) noexcept
{
    _scaled = glm::scale(coef);
}

void GraphicalObject::scale(const glm::vec3 &coef) noexcept
{
    _scaled *= glm::scale(coef);
}

void GraphicalObject::bindTexture(std::shared_ptr<Texture> texturePtr) noexcept
{
    _texturePtr = texturePtr;
}

void GraphicalObject::bindVertexArray(std::shared_ptr<VertexArray> ptr) noexcept
{
    _vertexArrayPtr = ptr;
}

void GraphicalObject::drawSelf() {}

void GraphicalObject::draw(glm::mat4 scaled,
                           glm::mat4 rotated,
                           glm::mat4 translated)
{
    rotated *= _rotated;
    translated *= _translated;
    scaled *= _scaled;
    _modelMatrix = translated * rotated * scaled;
    _modelMatrixUniform.load();
    if (_texturePtr)
    {
        _texturePtr->bind();
    }
    if (_vertexArrayPtr)
    {
        _vertexArrayPtr->execute();
    }
    drawSelf();
    for (auto subObject : _subObjects)
    {
        subObject->draw(scaled, rotated, translated);
    }
    exception::OpenGLException::handle();
}
} // namespace ample::graphics
