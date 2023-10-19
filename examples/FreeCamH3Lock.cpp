#include <Engine.hpp>
#include <Components.hpp>
#include <Utilities.hpp>
#include <Parsers.hpp>

int main()
{
  Engine engine(1280, 1024, "NEGE");
  Scene *scene = SceneParser::loadScene(relpath("../res/obj/scene.scn"));

  FOR_ANY_SCENE_TYPED(scene, {
    MovementController<any_scene_ttype> *camera = new MovementController<any_scene_ttype>(1280, 1024);
    *camera->transformation += glm::vec3(0, 0.15, 0);
    camera->enable();

    any_scene->setCamera(camera);
    engine.addEvent(camera);

    // any_scene->lights.addLight(DirectionalLight(glm::vec3(0.0, 0.8, -0.2), glm::vec3(0.8)));
    any_scene->lights.addLight(PointLight(glm::vec3(0.301, 0.45, 0.301), glm::vec3(1.0), 0.0,0.5));

    engine.changeScene(any_scene);
    engine.loop();
    delete camera;
  })
  delete scene;
  return 0;
}