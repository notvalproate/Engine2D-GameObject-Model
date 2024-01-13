#include "Core.hpp"
#include "behaviours/Player.cpp"
#include "behaviours/PlayerController.cpp"

class MyScene : public Scene {
public:
    using Scene::Scene;

    void SetupScene() override {
        auto PlayerObject = CreateGameObject("Player");
        PlayerObject->AddComponent<Player, PlayerController>();
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