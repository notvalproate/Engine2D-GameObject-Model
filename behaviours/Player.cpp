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

    void Start() override {
        std::cout << "Player instance id is " << gameObject->m_SceneInstanceID << std::endl;
        Transform* Weapon = transform->Find("Weapon");
        Transform* Trinket = nullptr;

        if(Weapon) {
            std::cout << "Weapon instance id is " << Weapon->gameObject->m_SceneInstanceID << std::endl;
            Trinket = Weapon->Find("Trinket");
        }

        if(Trinket) {
            std::cout << "Trinket instance id is " << Trinket->gameObject->m_SceneInstanceID << std::endl;
        }
    }

    void Update() override {
        transform->position.x += 1;
        std::cout << "Player Moved to " << transform->position.x << std::endl;
    }

    void HitWall() {
        health--;
        std::cout << "Player health reduced to " <<  health << std::endl;
        if(health == 0) {
            Death();
        }
    }

    void Death() {
        std::cout << "Player died! Game Over!" << std::endl;
        GameObject::Destroy(gameObject);
    }

    unsigned int health;
};