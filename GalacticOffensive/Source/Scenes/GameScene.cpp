#include "GameScene.hpp"

#include <SFML/Graphics.hpp>

#include "../../../ImasiEngine/Source/Experimental/Helpers.hpp"
#include "../../../ImasiEngine/Source/Utils/Logger.hpp"
#include "../../../ImasiEngine/Source/DeleteMe/MeshLoader.hpp"

GameScene::GameScene()
{
    {
        sf::Font font;
        if (!font.loadFromFile("C:/Windows/Fonts/Arial.ttf"))
        {
            ImasiEngine::SceneEvent event;
            event.type = ImasiEngine::SceneEventType::End;
            this->pushEvent(event);
            ImasiEngine::Logger::out << "[MenuScene] Could not load font Arial" << std::endl;
        }
        _resources.set("arial", std::move(font));
    }

    _text.setString("Press SPACE to shoot");
    _text.setFont(*_resources.getFont("arial"));
    _text.setCharacterSize(15);
    _text.setFillColor(sf::Color::Yellow);
    _text.setPosition(10, 10);

    { // Load ship model
        ImasiEngine::Model shipModel;
        ImasiEngine::Mesh* shipMesh;

        shipMesh = loadMesh("Resources/katarina.fbx");
        _resources.set("ship", std::move(*shipMesh));
        delete shipMesh;

        ImasiEngine::ColorTexture2D texture;
        texture.loadFromFile("Resources/katarina_diffuse.png");
        _resources.set("ship", std::move(texture));

        ImasiEngine::Material myMaterial;
        myMaterial.diffuseMap = _resources.getColorTexture("ship");
        _resources.set("ship", std::move(myMaterial));

        shipModel.material = _resources.getMaterial("ship");
        shipModel.mesh = _resources.getMesh("ship");

        _resources.set("ship", std::move(shipModel));
    }

    _ship.setModel(_resources.getModel("ship"));
    _ship.setPosition(glm::vec3(250, 250, 0));
    _ship.setScale(glm::vec3(10, 10, 10));

    _camera.setFieldOfView(90);
    _camera.setAspectRatio(1);
    _camera.setPosition(glm::vec3(250, 250, 250));


    for (int i = 0; i < 10; i++)
    {
        float velocityAngle = (float)rand();
        PhysicsEntity newAst;
        newAst.position.x = rand() % 480 + 10;
        newAst.position.y = rand() % 480 + 10;
        newAst.velocity.x = cos(velocityAngle) * 200.f;
        newAst.velocity.y = sin(velocityAngle) * 200.f;
        _asteroids.push_back(newAst);
    }
}

void GameScene::processWindowEvent(const sf::Event& event)
{
    _inputHandler.processWindowEvent(event);

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Key::Escape)
        {
            ImasiEngine::SceneEvent event;
            event.type = ImasiEngine::SceneEventType::End;
            pushEvent(event);
        }
        else if (event.key.code == sf::Keyboard::Key::Space)
        {
            sf::Vector2f mouse(_inputHandler.getMousePosition().x, 500 - _inputHandler.getMousePosition().y);
            sf::Vector2f dif = mouse - sf::Vector2f(_ship.getPosition().x, _ship.getPosition().y);
            float length = dif.x*dif.x + dif.y*dif.y;
            if (dif.x*dif.x + dif.y*dif.y == 0.f) {
                dif.x = 0;
                dif.y = 1;
            }
            else
            {
                dif /= sqrt(length);
            }

            PhysicsEntity bullet;
            bullet.position.x = _ship.getPosition().x;
            bullet.position.y = _ship.getPosition().y;
            bullet.velocity = dif * 500.f ; // 500 px/s
            _bullets.push_back(bullet);
        }
    }
}

void GameScene::processEngineEvent(const ImasiEngine::EngineEvent& event)
{
    switch (event.type)
    {
        case ImasiEngine::EngineEventType::Start:
        case ImasiEngine::EngineEventType::ChildEnded:
            break;
    }
}

void GameScene::update(const float deltaTime)
{
    if (_inputHandler.isMouseButtonPressed(sf::Mouse::Button::Left))
    {
        sf::Vector2f mouse = sf::Vector2f(_inputHandler.getMousePosition());
        mouse.y = 500 - mouse.y;

        sf::Vector2f dif = mouse - sf::Vector2f(_ship.getPosition().x, _ship.getPosition().y);
        float length = dif.x*dif.x + dif.y*dif.y;
        if (dif.x*dif.x + dif.y*dif.y == 0.f) {
            dif.x = 0;
            dif.y = 1;
        }
        else
        {
            dif /= sqrt(length);
        }
        _shipVelocity.x += dif.x * deltaTime * 20.f;
        _shipVelocity.y += dif.y * deltaTime * 20.f;
    }
    _ship.setPosition(_ship.getPosition() + glm::vec3(_shipVelocity, 0));

    if (_ship.getPosition().x < 5 || _ship.getPosition().x > 495)
    {
        if (_ship.getPosition().x < 5)
        {
            _ship.setPosition(glm::vec3(10 - _ship.getPosition().x, _ship.getPosition().y, 0));
        }
        else
        {
            _ship.setPosition(glm::vec3(495 * 2 - _ship.getPosition().x, _ship.getPosition().y, 0));
        }
        _shipVelocity.x = -_shipVelocity.x;
    }

    if (_ship.getPosition().y < 5 || _ship.getPosition().y > 495)
    {
        if (_ship.getPosition().y < 5)
        {
            _ship.setPosition(glm::vec3(_ship.getPosition().x, 10 - _ship.getPosition().y, 0));
        }
        else
        {
            _ship.setPosition(glm::vec3(_ship.getPosition().x, 495 * 2 - _ship.getPosition().y, 0));
        }
        _shipVelocity.y = -_shipVelocity.y;
    }

    if (glm::length(_shipVelocity) > 5.f)
    {
        _shipVelocity = glm::normalize(_shipVelocity) * 5.f;
    }
    _shipVelocity.x *= 0.99;
    _shipVelocity.y *= 0.99;


    if (abs(_shipVelocity.x) + abs(_shipVelocity.y) < 0.1)
    {
        _shipVelocity.x = _shipVelocity.y = 0;
    }

    sf::Vector2f mouseSf(_inputHandler.getMousePosition());
    glm::vec3 mouse(mouseSf.x, 500 - mouseSf.y, 0);

    glm::vec2 mouseRelative(mouse.x - _ship.getPosition().x, mouse.y - _ship.getPosition().y);

    if (mouseRelative.x != 0 || mouseRelative.y == 0)
    {
        glm::vec3 dif = mouse - _ship.getPosition();

        float temp = (mouseRelative.x*_shipVelocity.x + mouseRelative.y*_shipVelocity.y)
            / (glm::length(mouseRelative)*
                glm::length(_shipVelocity));
        if (temp <= -1.f)
            temp += 0.01;
        if (temp >= 1.f)
            temp -= 0.01;
        float angle = acos(temp) / 2.f;

        _ship.setRotation(glm::vec3(
            90,
            135 - atan2(dif.x, dif.y),
            (_shipVelocity.x == 0 && _shipVelocity.y == 0 ?
                0 : angle * glm::length(_shipVelocity) / 5.f)
        ));
    }

    for (auto it = _bullets.begin(); it != _bullets.end();)
    {
        it->position += it->velocity * deltaTime;
        if (it->position.x < -2 || it->position.y < -2 ||
            it->position.x > 502 || it->position.y > 502)
        {
            it = _bullets.erase(it);
        }
        else
            ++it;
    }

    for (PhysicsEntity& ast : _asteroids)
    {
        ast.position += ast.velocity * deltaTime;

        if (ast.position.x < 10 || ast.position.x > 490)
        {
            if (ast.position.x < 10)
            {
                ast.position.x = 20 - ast.position.x;
            }
            else
            {
                ast.position.x = 490 * 2 - ast.position.x;
            }

            ast.velocity.x = -ast.velocity.x;
        }

        if (ast.position.y < 10 || ast.position.y > 490)
        {
            if (ast.position.y < 10)
            {
                ast.position.y = 20 - ast.position.y;
            }
            else
            {
                ast.position.y = 490 * 2 - ast.position.y;
            }

            ast.velocity.y = -ast.velocity.y;
        }
    }

    for (auto it1 = _bullets.begin(); it1 != _bullets.end();)
    {
        bool destroy = false;

        for (auto it2 = _asteroids.begin(); it2 != _asteroids.end();)
        {
            if (std::hypotf(it1->position.x - it2->position.x,
                it1->position.y - it2->position.y) < 12)
            {
                destroy = true;
                it2 = _asteroids.erase(it2);
                break;
            }
            else
                it2++;
        }

        if (destroy)
        {
            it1 = _bullets.erase(it1);
        }
        else
            it1++;
    }
}

void GameScene::render(sf::RenderTarget& renderTarget)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderTarget.pushGLStates();

    renderTarget.clear();

    sf::CircleShape circle;
    circle.setOutlineThickness(0);

    circle.setRadius(4);
    circle.setFillColor(sf::Color::Green);

    for (const PhysicsEntity& bullet : _bullets)
    {
        sf::Vector2f bulletPosition = bullet.position - sf::Vector2f(2, 2);
        bulletPosition.y = 500 - bulletPosition.y;
        circle.setPosition(bulletPosition);
        renderTarget.draw(circle);
    }

    circle.setRadius(10);

    for (const PhysicsEntity& ast : _asteroids)
    {
        sf::Vector2f astPosition = ast.position - sf::Vector2f(10, 10);
        astPosition.y = 500 - astPosition.y;
        circle.setPosition(astPosition);
        renderTarget.draw(circle);
    }

    renderTarget.popGLStates();

    /*_ship.setPosition(glm::vec3(250, 0, 0));
    _ship.setRotation(glm::vec3(0, 0, 0));
    _ship.setScale(glm::vec3(80, 80, 80));*/

    _renderer.clear();
    _renderer.add(&_ship);
    _renderer.render(_camera);


    renderTarget.pushGLStates();
    renderTarget.draw(_text);
    renderTarget.popGLStates();
}