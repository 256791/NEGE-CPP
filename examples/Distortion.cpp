#include <Engine.hpp>
#include <Components.hpp>
#include <Utilities.hpp>
#include <Parsers.hpp>

int main(int argc, char *argv[])
{
  Engine engine(1280, 1024, "NEGE");
  Scene *scene = SceneParser::loadScene(relpath("../res/obj/scene.scn"));
  ImageCaptutre *recorder = new ImageCaptutre("./render/Distortion/keyframes/capture", ImageCaptutre::Mode::INTERVAL, 30);
  ImageCaptutre *recorder_sec = new ImageCaptutre("./render/Distortion/frames/capture", ImageCaptutre::Mode::CONTINOUS);
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

    for (int i = 0; i < 8 * 30; i++)
    {
      engine.renderFrame();
      engine.handleEvents();
      switch (i / 30)
      {
      case 0:
        *camera->transformation += glm::vec3(0.007, 0.0, 0.007);
        camera->yaw += 0.001;
        break;
      case 2:
        *camera->transformation += glm::vec3(0.0, 0.0, -0.01);
        camera->pitch -= 0.01;
        break;
      case 3:
      case 4:
        *camera->transformation += glm::vec3(0.0, 0.01, -0.005);
        camera->pitch -= 0.02;
        break;
      case 5:
        *camera->transformation += glm::vec3(0.0, 0.008, -0.004);
        break;
      case 6:
        *camera->transformation += glm::vec3(0.0, 0.004, 0.0);
        break;
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