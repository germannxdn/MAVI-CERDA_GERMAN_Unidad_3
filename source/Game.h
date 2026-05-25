// Game.h
#pragma once
#include "PhysicsWorld.h"
#include "Constants.h"

class Game {
private:
    PhysicsWorld physicsWorld;
    bool running;

public:
    Game();
    ~Game();

    void Run();
    void Update();
    void Draw();
    void DrawUI();

private:
    void HandleInput();
};