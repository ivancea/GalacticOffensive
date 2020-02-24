#ifndef GO_MENUSCENE_HPP
#define GO_MENUSCENE_HPP

#include "../../ImasiEngine/Source/Engine/Scene.hpp"

#include "../../ImasiEngine/Source/Experimental/InputHandler.hpp"

class MenuScene : public ImasiEngine::Scene
{
private:

    ImasiEngine::InputHandler _inputHandler;
    sf::Font _font;

    sf::RectangleShape _buttonStart;
    sf::Text _text;

    bool _childCreated;

public:
    
    MenuScene();

    virtual void processWindowEvent(const sf::Event& event) override;
    virtual void processEngineEvent(const ImasiEngine::EngineEvent& event) override;
    virtual void update(const float deltaTime) override;

    virtual void render(sf::RenderTarget& renderTarget) override;

};

#endif