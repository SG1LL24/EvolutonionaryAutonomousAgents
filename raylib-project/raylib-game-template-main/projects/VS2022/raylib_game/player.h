//
// Created by Simarbir G on 10/3/2023.
//

#pragma once
#include "raylib.h"
#include "raymath.h"

class Player
{
public:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    float maxSpeed;
    float maxForce;

    Player(float x, float y);
    void movePlayer();
    void addForce(Vector2 force);
    void findTarget(Vector2 target);
    void boostMaxSpeed(float m);
};