#ifndef GO_GAMESCENE_HPP
#define GO_GAMESCENE_HPP

#include <list>

#include "../../ImasiEngine/Source/Engine/Scene.hpp"
#include "../../ImasiEngine/Source/Graphics/Entities/Entity.hpp"
#include "../../ImasiEngine/Source/Graphics/Cameras/Camera.hpp"
#include "../../ImasiEngine/Source/Graphics/Renderers/InstancedRenderer.hpp"
#include "../../ImasiEngine/Source/Resources/ResourceContainer.hpp"
#include "../../ImasiEngine/Source/Experimental/InputHandler.hpp"


// Temporal

struct PhysicsEntity
{
    sf::Vector2f position;
    sf::Vector2f velocity;
};


class GameScene : public ImasiEngine::Scene
{
private:

    ImasiEngine::InputHandler _inputHandler;
    ImasiEngine::ResourceContainer<> _resources;

    sf::Text _text;

    ImasiEngine::InstancedRenderer _renderer;
    ImasiEngine::Camera _camera;

    ImasiEngine::Entity _ship;
    glm::vec2 _shipVelocity;

    std::list<PhysicsEntity> _bullets;
    std::list<PhysicsEntity> _asteroids;

public:
    
    GameScene();

    virtual void processWindowEvent(const sf::Event& event) override;
    virtual void processEngineEvent(const ImasiEngine::EngineEvent& event) override;
    virtual void update(const float deltaTime) override;

    virtual void render(sf::RenderTarget& renderTarget) override;

};

#endif