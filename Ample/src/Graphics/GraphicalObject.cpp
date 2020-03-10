#define GLM_ENABLE_EXPERIMENTAL
#define GL_GLEXT_PROTOTYPES 1

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GraphicalObject.h"
#include "ShaderProcessor.h"
#include "Exception.h"

namespace ample::graphics
{
GraphicalObject::GraphicalObject()
    : _programId(shaders::ShaderProcessor::instance().getProgramId()),
      _modelMatrixId(glGetUniformLocation(_programId, "model_matrix")) {}

void GraphicalObject::addSubObject(GraphicalObject &object)
{
    activity::Behaviour::addBehaviour(object);
    _subObjects.push_back(&object);
}

float GraphicalObject::getX() const { return _translated[0][0]; }
float GraphicalObject::getY() const { return _translated[1][1]; }
float GraphicalObject::getZ() const { return _translated[2][2]; }

void GraphicalObject::setTranslate(glm::vec3 vector)
{
    _translated = glm::translate(vector);
}
void GraphicalObject::translate(glm::vec3 vector)
{
    _translated *= glm::translate(vector);
}

void GraphicalObject::setRotate(glm::vec3 axis, float angle)
{
    _rotated = glm::rotate(glm::radians(angle), axis);
}
void GraphicalObject::rotate(glm::vec3 axis, float angle)
{
    _rotated = glm::rotate(_rotated, glm::radians(angle), axis);
}

void GraphicalObject::draw(glm::mat4 rotated,
                           glm::mat4 translated)
{
    rotated *= _rotated;
    translated *= _translated;
    glm::mat4 modelMatrix = translated * rotated;
    glUniformMatrix4fv(_modelMatrixId, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    exception::OpenGLException::handle();
    drawSelf(modelMatrix);
    for (auto subObject : _subObjects)
    {
        subObject->draw(rotated, translated);
    }
    exception::OpenGLException::handle();
}
} // namespace ample::graphics
