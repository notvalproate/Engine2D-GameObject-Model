#include "Core.hpp"
#include "behaviours/PositionDebug.cpp"
#include "behaviours/LocalPositionDebug.cpp"

class MyScene : public Scene {
public:
    using Scene::Scene;

    void SetupScene() override {
        std::cout << "Setup Started" << std::endl;

        auto Parent = CreateGameObject("Parent");
        Parent->AddComponent<PositionDebug>();
        Parent->tag = "Parent";

        auto Child = CreateGameObject("Child");
        Child->AddComponent<PositionDebug, LocalPositionDebug>();
        Child->tag = "Child";

        Child->transform.SetParent(&Parent->transform);

        auto childTransform = Parent->transform.Find("Child");

        if(childTransform) {
            std::cout << "Found " << childTransform->name << std::endl;
        }

        std::cout << std::boolalpha;

        std::cout << Child->transform.IsChildOf(&Parent->transform) << std::endl;

        // Parent.transform.DetachChildren();

        std::cout << Parent->transform.IsChildOf(&Child->transform) << std::endl;

        Child->transform.Translate(Vector2D(1.0, 1.0));
        Parent->transform.Translate(Vector2D(5.0, 5.0));

        std::cout << "Setup Complete" << std::endl;
    }
};

int main() {
    MyScene testScene("Test");

    testScene.SetupScene();

    testScene.Start();
    std::cout << "updating " << std::endl;
    testScene.Update();

    return 0;
}