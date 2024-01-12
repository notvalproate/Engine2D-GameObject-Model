#include "Core.hpp"
#include "behaviours/PositionDebug.cpp"
#include "behaviours/LocalPositionDebug.cpp"

class MyScene : public Scene {
public:
    void SetupScene() override {
        // GameObject& Parent = CreateGameObject("Parent");
        // Parent.AddComponent<PositionDebug>();
        // Parent.tag = "Parent";

        // GameObject& Child = CreateGameObject("Child");
        // Child.AddComponent<PositionDebug, LocalPositionDebug>();
        // Child.tag = "Child";

        // Child.transform.SetParent(Parent.transform);

        // auto childTransform = Parent.transform.Find("Child");

        // if(childTransform) {
        //     // delete childTransform; Does not work, since destructor is private and only GameObject is allowed.
        //     std::cout << "Found " << childTransform->name << std::endl;
        // }

        // std::cout << std::boolalpha;

        // std::cout << Child.transform.IsChildOf(Parent.transform) << std::endl;

        // // Parent.transform.DetachChildren();

        // std::cout << Parent.transform.IsChildOf(Child.transform) << std::endl;

        // Child.transform.Translate(Vector2D(1.0, 1.0));
        // Parent.transform.Translate(Vector2D(5.0, 5.0));
    }
};

int main() {
    MyScene scene;

    scene.Start();
    scene.Update();
    scene.Render();

    GameObject Parent("Parent", scene);
    Parent.AddComponent<PositionDebug>();
    Parent.tag = "Parent";

    GameObject Child("Child", scene);
    Child.AddComponent<PositionDebug, LocalPositionDebug>();
    Child.tag = "Child";

    Child.transform.SetParent(Parent.transform);

    auto childTransform = Parent.transform.Find("Child");

    if(childTransform) {
        // delete childTransform; Does not work, since destructor is private and only GameObject is allowed.
        std::cout << "Found " << childTransform->name << std::endl;
    }

    std::cout << std::boolalpha;

    std::cout << Child.transform.IsChildOf(Parent.transform) << std::endl;

    // Parent.transform.DetachChildren();

    std::cout << Parent.transform.IsChildOf(Child.transform) << std::endl;

    Child.transform.Translate(Vector2D(1.0, 1.0));
    Parent.transform.Translate(Vector2D(5.0, 5.0));
    

    return 0;
}