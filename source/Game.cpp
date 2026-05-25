// Game.cpp
#include "Game.h"
#include "raylib.h"

Game::Game() : running(true) {
    InitWindow(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT,
        "MAVI II - Unidad 3: Puente Colgante");
    SetTargetFPS(60);
}

Game::~Game() {
    CloseWindow();
}

void Game::HandleInput() {
    }

void Game::Update() {
    physicsWorld.Update();
    }

void Game::DrawUI() {
    DrawText("PUENTE COLGANTE - MAVI II Unidad 3", 10, 10, 25, RAYWHITE);
    DrawText("Revolute Joints: conectan los segmentos", 10, 50, 16, YELLOW);
    DrawText("Distance Joints: cables de los pilares", 10, 70, 16, YELLOW);
    DrawText("El puente se mueve y oscila", 10, 90, 16, LIGHTGRAY);
    
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(Constants::BACKGROUND);

    physicsWorld.Draw();
    DrawUI();

    EndDrawing();
}

void Game::Run() {
    while (running && !WindowShouldClose()) {
        HandleInput();
        Update();
        Draw();
    }
}