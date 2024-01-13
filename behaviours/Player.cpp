#pragma once
#include "../Core.hpp"

class PlayerController;

class Player : public Behaviour {
public:
    using Behaviour::Behaviour;

    void Awake() override {
        std::cout << "Player has awoken!" << std::endl;
        health = 10;
    }

    void Update() override {
        transform->position.x += 1;
        std::cout << "Player Moved to " << transform->position.x << std::endl;
    }

    void HitWall() {
        health--;
        std::cout << "Player health reduced to " <<  health << std::endl;
        if(health == 0) {
            enabled = false;
            Death();
        }
    }

    void Death() {
        std::cout << "Player died! Game Over!" << std::endl;
    }

    unsigned int health;
};