#include <Engine.hpp>
#include <Components.hpp>
#include <Utilities.hpp>
#include <Parsers.hpp>

int main(int argc, char *argv[])
{
  Engine engine(1280, 1024, "NEGE");
  Scene *scene = SceneParser::loadScene(relpath("../res/obj/e_scene.scn"));
  ImageCaptutre *recorder = new ImageCaptutre("./render/Space_E3/keyframes/capture", ImageCaptutre::Mode::INTERVAL, 30);
  ImageCaptutre *recorder_sec = new ImageCaptutre("./render/Space_E3/frames/capture", ImageCaptutre::Mode::CONTINOUS);
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
    any_scene->lights.addLight(PointLight(glm::vec3(0.38, 0.45, 0.38), glm::vec3(1.2), 0.1,0.7,0.09));

    *camera->transformation += glm::vec3(0, 0.15, 0);

        for (int i = 0; i < 6 * 60; i++)
    {
      engine.renderFrame();
      engine.handleEvents();
      switch (i / 30)
      {
      case 0:
      case 1:
        *camera->transformation += glm::vec3(0.009, 0.0, 0.009);
        camera->yaw += 0.013;
        break;
      case 2:
      case 3:
        *camera->transformation += glm::vec3(-0.016, 0.02, -0.016);
        camera->pitch -= 0.02617993877;
        break;
      case 5:
      case 6:
        *camera->transformation += glm::vec3(-0.018, 0.00, -0.0);
        camera->yaw -= 0.02617993877;
        break;
      case 7:
      case 8:
        *camera->transformation += glm::vec3(0.0, -0.02, 0.0);
        camera->pitch += 0.02617993877;
        camera->yaw -= 0.014;
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