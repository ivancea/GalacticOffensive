#include "MenuScene.hpp"

#include <SFML/Graphics.hpp>

#include "../../../ImasiEngine/Source/Utils/Logger.hpp"

#include "GameScene.hpp"

MenuScene::MenuScene()
{
    if (!_font.loadFromFile("C:/Windows/Fonts/Arial.ttf"))
    {
        ImasiEngine::SceneEvent event;
        event.type = ImasiEngine::SceneEventType::End;
        this->pushEvent(event);
        ImasiEngine::Logger::out << "[MenuScene] Could not load font Arial" << std::endl;
    }

    _childCreated = false;

    _buttonStart.setSize(sf::Vector2f(150, 50));
    _buttonStart.setOutlineThickness(2);
    _buttonStart.setOutlineColor(sf::Color::Black);

    _text.setString("Start");
    _text.setFont(_font);
    _text.setCharacterSize(30);
    _text.setFillColor(sf::Color::Black);
}

void MenuScene::processWindowEvent(const sf::Event& event)
{
    _inputHandler.processWindowEvent(event);

    if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Button::Left)
        {
            if (_buttonStart.getGlobalBounds().contains(sf::Vector2f(_inputHandler.getMousePosition())))
            {
                ImasiEngine::SceneEvent event;
                event.type = ImasiEngine::SceneEventType::NewScene;
                event.newScene = new GameScene();
                this->pushEvent(event);
                _childCreated = true;
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Key::Escape)
        {
            ImasiEngine::SceneEvent event;
            event.type = ImasiEngine::SceneEventType::End;
            pushEvent(event);
        }
    }
}

void MenuScene::processEngineEvent(const ImasiEngine::EngineEvent& event)
{
    switch (event.type)
    {
        case ImasiEngine::EngineEventType::Start:
        case ImasiEngine::EngineEventType::ChildEnded:
            _childCreated = false;
            _inputHandler.resetStates();
            break;
    }
}

void MenuScene::update(const float deltaTime)
{
}

void MenuScene::render(sf::RenderTarget& renderTarget)
{
    if(!_childCreated)
    {
        renderTarget.pushGLStates();
        renderTarget.clear();

        sf::Vector2f center = sf::Vector2f(renderTarget.getSize()) / 2.f;

        _buttonStart.setPosition(center - _buttonStart.getSize() / 2.f);
        _text.setPosition(
            center.x - _text.getGlobalBounds().width / 2,
            center.y - _text.getGlobalBounds().height / 2 - 10.f
        );

        if (!_buttonStart.getGlobalBounds().contains(sf::Vector2f(_inputHandler.getMousePosition())))
        {
            _buttonStart.setFillColor(sf::Color::Yellow);
        }
        else if(_inputHandler.isMouseButtonPressed(sf::Mouse::Button::Left))
        {
            _buttonStart.setFillColor(sf::Color::Cyan);
        }
        else
        {
            _buttonStart.setFillColor(sf::Color::Green);
        }

        renderTarget.draw(_buttonStart);
        renderTarget.draw(_text);

        renderTarget.popGLStates();
    }
}