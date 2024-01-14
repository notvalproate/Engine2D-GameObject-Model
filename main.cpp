#include "Core.hpp"
#include "behaviours/Player.hpp"
#include "behaviours/PlayerController.hpp"

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
            GameObject::Destroy(gameObject);
        }
    }

    void Death() {
        std::cout << "Player died! Game Over!" << std::endl;
    }

    unsigned int health;
}; 

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

class MyScene : public Scene {
public:
    using Scene::Scene;

    void SetupScene() override {
        auto PlayerObject = CreateGameObject("Player");
        PlayerObject->AddComponent<Player, PlayerController>();

        auto WeaponObject = CreateGameObject("Weapon");
        WeaponObject->transform.SetParent(PlayerObject);

        auto TrinketObject = CreateGameObject("Trinket");
        TrinketObject->transform.SetParent(WeaponObject);
    }
};

int main() {
    MyScene testScene("Test");

    testScene.SetupScene();
    testScene.Start();

    for(int i = 0; i < 100; i++) {
        testScene.Update();
        testScene.Render();
    }

    return 0;
}