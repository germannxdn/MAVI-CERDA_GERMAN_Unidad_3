#include "PhysicsWorld.h"
#include <cmath>

PhysicsWorld::PhysicsWorld()
    : world(b2Vec2(0.0f, 9.8f)), groundBody(nullptr), leftPillar(nullptr), rightPillar(nullptr) {

    // -------------------------
    // 1. SUELO ESTÁTICO
    // -------------------------
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(Constants::GROUND_WIDTH / 2.0f, Constants::GROUND_Y);
    groundBody = world.CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(Constants::GROUND_WIDTH / 2.0f, 0.2f);
    groundBody->CreateFixture(&groundShape, 0.0f);

    // -------------------------
    // 2. PILARES (estáticos)
    // -------------------------
    b2BodyDef pillarDef;
    pillarDef.type = b2_staticBody;

    // Pilar izquierdo
    pillarDef.position.Set(Constants::LEFT_PILLAR_X, Constants::PILLAR_Y);
    leftPillar = world.CreateBody(&pillarDef);
    b2PolygonShape pillarShape;
    pillarShape.SetAsBox(Constants::PILLAR_WIDTH / 2.0f, Constants::PILLAR_HEIGHT / 2.0f);
    leftPillar->CreateFixture(&pillarShape, 0.0f);

    // Pilar derecho
    pillarDef.position.Set(Constants::RIGHT_PILLAR_X, Constants::PILLAR_Y);
    rightPillar = world.CreateBody(&pillarDef);
    rightPillar->CreateFixture(&pillarShape, 0.0f);

    // -------------------------
    // 3. SEGMENTOS DEL PUENTE
    // -------------------------
    float startX = Constants::BRIDGE_START_X;
    float segmentSpacing = Constants::SEGMENT_WIDTH;

    for (int i = 0; i < Constants::NUM_SEGMENTS; i++) {
        float x = startX + i * segmentSpacing;
        float y = Constants::BRIDGE_Y;

        auto segment = std::make_unique<BridgeSegment>(
            &world, x, y,
            Constants::SEGMENT_WIDTH,
            Constants::SEGMENT_HEIGHT,
            Constants::SEGMENT_COLOR
        );
        segments.push_back(std::move(segment));
    }

    // -------------------------
    // 4. JOINTS ENTRE SEGMENTOS (Revolute Joint)
    // -------------------------
    for (int i = 0; i < Constants::NUM_SEGMENTS - 1; i++) {
        b2RevoluteJointDef revJointDef;
        revJointDef.Initialize(
            segments[i]->GetBody(),
            segments[i + 1]->GetBody(),
            segments[i]->GetBody()->GetWorldCenter()
        );
        revJointDef.collideConnected = false;  // Los segmentos no chocan entre sí

        b2RevoluteJoint* joint = (b2RevoluteJoint*)world.CreateJoint(&revJointDef);
        revoluteJoints.push_back(joint);
    }

    // -------------------------
    // 5. CABLES (Distance Joints) de los pilares a los segmentos
    // -------------------------
    // Cable del pilar izquierdo al primer segmento
    b2DistanceJointDef cableLeft;
    cableLeft.Initialize(
        leftPillar,
        segments[0]->GetBody(),
        b2Vec2(Constants::LEFT_PILLAR_X, Constants::PILLAR_Y - Constants::PILLAR_HEIGHT / 2),
        segments[0]->GetBody()->GetWorldCenter()
    );
    
    cableJoints.push_back((b2DistanceJoint*)world.CreateJoint(&cableLeft));

    // Cable del pilar derecho al último segmento
    b2DistanceJointDef cableRight;
    cableRight.Initialize(
        rightPillar,
        segments[Constants::NUM_SEGMENTS - 1]->GetBody(),
        b2Vec2(Constants::RIGHT_PILLAR_X, Constants::PILLAR_Y - Constants::PILLAR_HEIGHT / 2),
        segments[Constants::NUM_SEGMENTS - 1]->GetBody()->GetWorldCenter()
    );
    
    cableJoints.push_back((b2DistanceJoint*)world.CreateJoint(&cableRight));
}

PhysicsWorld::~PhysicsWorld() {
    // Los joints y cuerpos se destruyen con el mundo
}

void PhysicsWorld::Update() {
    world.Step(1.0f / 60.0f, 8, 3);
}

void PhysicsWorld::Draw() {
    // Dibujar suelo
    float groundYPixels = Constants::ToPixels(Constants::GROUND_Y);
    DrawRectangle(0, (int)groundYPixels, Constants::SCREEN_WIDTH, 20, DARKGREEN);

    // Dibujar pilares
    float leftX = Constants::ToPixels(Constants::LEFT_PILLAR_X);
    float leftY = Constants::ToPixels(Constants::PILLAR_Y);
    float pillarWPixels = Constants::ToPixels(Constants::PILLAR_WIDTH);
    float pillarHPixels = Constants::ToPixels(Constants::PILLAR_HEIGHT);

    DrawRectangle(leftX - pillarWPixels / 2, leftY - pillarHPixels / 2,
        pillarWPixels, pillarHPixels, Constants::PILLAR_COLOR);

    float rightX = Constants::ToPixels(Constants::RIGHT_PILLAR_X);
    DrawRectangle(rightX - pillarWPixels / 2, leftY - pillarHPixels / 2,
        pillarWPixels, pillarHPixels, Constants::PILLAR_COLOR);

    // Dibujar segmentos del puente
    for (const auto& segment : segments) {
        segment->Draw();
    }

    // Dibujar cables (líneas para visualizar)
    DrawJoints();
}

void PhysicsWorld::DrawJoints() {
    // Dibujar cables como líneas blancas
    for (auto* joint : cableJoints) {
        b2Vec2 anchorA = joint->GetAnchorA();
        b2Vec2 anchorB = joint->GetAnchorB();

        float x1 = Constants::ToPixels(anchorA.x);
        float y1 = Constants::ToPixels(anchorA.y);
        float x2 = Constants::ToPixels(anchorB.x);
        float y2 = Constants::ToPixels(anchorB.y);

        DrawLine((int)x1, (int)y1, (int)x2, (int)y2, LIGHTGRAY);

        // Dibujar círculos en los puntos de anclaje
        DrawCircle((int)x1, (int)y1, 4, RED);
        DrawCircle((int)x2, (int)y2, 4, RED);
    }
}