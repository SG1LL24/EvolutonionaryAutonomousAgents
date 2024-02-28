//
// Created by Simarbir G on 10/9/2023.
//
#include "raylib.h"
#include "raymath.h"
#include "player.h"


Player::Player(float x, float y)
{
    position = Vector2({ x, y });
    velocity = Vector2({ 0, 0 });
    acceleration = Vector2({ 0, 0 });

    maxSpeed = 3;
    maxForce = 0.2;
}

void Player::movePlayer()
{
    velocity = Vector2Add(velocity, acceleration);
    velocity = Vector2ClampValue(velocity, 0, maxSpeed);
    position = Vector2Add(position, velocity);
    acceleration = Vector2Multiply(acceleration, Vector2({0,0}));
}

void Player::addForce(Vector2 force)
{
    acceleration = Vector2Add(acceleration, force);
}

void Player::findTarget(Vector2 target)
{
    Vector2 desiredVelocity = Vector2Subtract(target, position);
    desiredVelocity = Vector2Normalize(desiredVelocity);
    desiredVelocity = Vector2Multiply(desiredVelocity, Vector2({maxSpeed, maxSpeed}));

    Vector2 seekingForce = Vector2Subtract(desiredVelocity, velocity);
    seekingForce = Vector2ClampValue(seekingForce, 0, maxForce);

    addForce(seekingForce);
}

void Player::boostMaxSpeed(float m)
{
    maxSpeed += m;
}