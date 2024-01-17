#pragma once
#include "../Core.hpp"
#include "Player.hpp"

class PlayerController : public Behaviour {
public:
    using Behaviour::Behaviour;

    std::unique_ptr<Component> Clone() const {
        return std::make_unique<PlayerController>(*this);
    }

    // Your Code Goes Here

    void Start() override {
        playerScript = gameObject->GetComponent<Player>();
        std::cout << "Finding other player" << std::endl;
        otherPlayer = FindObjectByName("Player #3");
        if(otherPlayer) {
            std::cout << "Found other player!! " << otherPlayer->name << std::endl;
        }
    }

    void Update() override {
        if(transform->position.x == 4) {
            transform->position.x--;
            std::cout << *name << " Hit a wall!" << std::endl;
            playerScript->HitWall();
        }
    }

    Player* playerScript;
    Transform* playerTransform;
    GameObject* otherPlayer;
};
