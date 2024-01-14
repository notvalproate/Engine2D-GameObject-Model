#pragma once
#include "../Core.hpp"
#include "Player.hpp"

class PlayerController : public Behaviour {
public:
    using Behaviour::Behaviour;

    void Start() override {
        playerScript = gameObject->GetComponent<Player>();
    }

    void Update() override {
        if(transform->position.x == 4) {
            transform->position.x--;
            std::cout << "Player Hit a wall!" << std::endl;
            playerScript->HitWall();
        }
    }

    Player* playerScript;
    Transform* playerTransform;
};
