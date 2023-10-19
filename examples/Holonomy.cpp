#include <Engine.hpp>
#include <Components.hpp>
#include <Utilities.hpp>
#include <Parsers.hpp>

int main(int argc, char *argv[])
{
  Engine engine(1280, 1024, "NEGE");
  Scene *scene = SceneParser::loadScene(relpath("../res/obj/scene.scn"));
  ImageCaptutre *recorder = new ImageCaptutre("./render/Holonomy/keyframes/capture", ImageCaptutre::Mode::INTERVAL, 30);
  ImageCaptutre *recorder_sec = new ImageCaptutre("./render/Holonomy/frames/capture", ImageCaptutre::Mode::CONTINOUS);
  if(argc > 1){
    if(std::string(argv[1]) == "save"){
      engine.addEvent(recorder);
      engine.addEvent(recorder_sec);
    }
  }

  FOR_ANY_SCENE_TYPED(scene, {
    Camera<any_scene_ttype> *camera = new Camera<any_scene_ttype>(1280, 1024);
    any_scene->setCamera(camera);
    engine.changeScene(any_scene);

    // any_scene->lights.addLight(DirectionalLight(glm::vec3(0.0, 0.8, -0.2), glm::vec3(0.8)));
    any_scene->lights.addLight(PointLight(glm::vec3(0.301, 0.45, 0.301), glm::vec3(1.0), 0.0,0.5));

    *camera->transformation += glm::vec3(0, 0.15, 0);


    double s = 0.00885;
    for (int i = 0; i < 7 * 60; i++)
    {
      engine.renderFrame();
      engine.handleEvents();
      switch (i / 60)
      {
      case 1:
        *camera->transformation += glm::vec3(s, 0, 0);
        break;
      case 2:
        *camera->transformation += glm::vec3(0, 0, s);
        break;
      case 3:
        *camera->transformation += glm::vec3(-s, 0, 0);
        break;
      case 4:
        *camera->transformation += glm::vec3(0, 0, -s);
        break;
      case 5:
        *camera->transformation += glm::vec3(s, 0, 0);
        break;
      // case 7:
      //   *camera->transformation += glm::vec3(-m, 0.01, m);
      //   camera->pitch -= 0.02617993877;
      //   break;
      default:
        break;
      }
    }
    delete camera;
  })
  delete scene;
  delete recorder;
  delete recorder_sec;
  return 0;
}