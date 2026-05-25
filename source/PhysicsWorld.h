#pragma once
#include "box2d.h"
#include "raylib.h"
#include "BridgeSegment.h"
#include "Constants.h"
#include <vector>
#include <memory>

class PhysicsWorld {
private:
    b2World world;
    b2Body* groundBody;

    // Pilares (estáticos)
    b2Body* leftPillar;
    b2Body* rightPillar;

    // Segmentos del puente
    std::vector<std::unique_ptr<BridgeSegment>> segments;

    // Almacenar joints para poder dibujarlos
    std::vector<b2RevoluteJoint*> revoluteJoints;
    std::vector<b2DistanceJoint*> cableJoints;

public:
    PhysicsWorld();
    ~PhysicsWorld();

    void Update();
    void Draw();
    void DrawJoints();  // Para visualizar las conexiones

    // Getters
    b2World* GetWorld() { return &world; }
    const std::vector<std::unique_ptr<BridgeSegment>>& GetSegments() const { return segments; }
};