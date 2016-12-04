#include "Entity.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/detail/type_mat.hpp>

namespace ImasiEngine
{
    Entity::Entity()
        : _mustUpdateModelMatrix(false)
        , _modelMatrix(glm::mat4(1.f))
        , _position(glm::vec3(0.f, 0.f, 0.f))
        , _rotation(glm::vec3(0.f, 0.f, 0.f))
        , _scale(glm::vec3(1.f, 1.f, 1.f))
        , model(nullptr)
    {
    }

    Entity::~Entity()
    {
    }

    const glm::vec3& Entity::getPosition() const
    {
        return _position;
    }

    void Entity::setPosition(const glm::vec3& position)
    {
        _mustUpdateModelMatrix = true;
        _position = position;
    }

    const glm::vec3& Entity::getRotation() const
    {
        return _rotation;
    }

    void Entity::setRotation(const glm::vec3& rotation)
    {
        _mustUpdateModelMatrix = true;
        _rotation = rotation;
    }

    const glm::vec3& Entity::getScale() const
    {
        return _scale;
    }

    void Entity::setScale(const glm::vec3& scale)
    {
        _mustUpdateModelMatrix = true;
        _scale = scale;
    }

    const glm::mat4& Entity::getModelMatrix()
    {
        if (_mustUpdateModelMatrix)
        {
            glm::mat4 rotationMatrix(1.f);
            rotationMatrix = glm::rotate(rotationMatrix, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            rotationMatrix = glm::rotate(rotationMatrix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            rotationMatrix = glm::rotate(rotationMatrix, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

            _modelMatrix = glm::translate(_position) * rotationMatrix * glm::scale(_scale);
            _mustUpdateModelMatrix = false;
        }

        return _modelMatrix;
    }
}
