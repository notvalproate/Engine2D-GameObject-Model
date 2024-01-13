#pragma once
#include "../Core.hpp"
#include "Player.cpp"

class PlayerController : public Behaviour {
public:
    using Behaviour::Behaviour;

    void Start() override {
        playerScript = gameObject->GetComponent<Player>();
        playerTransform = playerScript->transform;
    }

    void Update() override {
        if(playerTransform->position.x == 4) {
            playerTransform->position.x--;
            std::cout << "Player Hit a wall!" << std::endl;
            playerScript->HitWall();
            if(playerScript->health == 0) {
                enabled = false;
            }
        }
    }

    Player* playerScript;
    Transform* playerTransform;
};