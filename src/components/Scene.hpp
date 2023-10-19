#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>
#include <mutex>

#include "Model.hpp"
#include "Entity.hpp"
#include "Camera.hpp"
#include "Light.hpp"

class Scene
{
public:
  virtual ~Scene(){};
};

template <class TransformationType>
class TypedScene : public Scene
{
public:
  TypedScene();
  ~TypedScene();
  void addModel(std::shared_ptr<Model> model);
  void addEntity(Entity<TransformationType> entity);
  void setCamera(Camera<TransformationType> *camera);

  std::vector<std::shared_ptr<Model>> models;
  std::vector<Entity<TransformationType>> entities;
  Camera<TransformationType> *camera;
  LightCollection lights;

private:
  std::mutex guard;
};

typedef TypedScene<E3Transformation> E3Scene;
typedef TypedScene<H3Transformation> H3Scene;
typedef TypedScene<H2ETransformation> H2EScene;
typedef TypedScene<H3LockedTransformation> H3LockScene;

#define FOR_ANY_SCENE(currentScene, code)                                      \
  if (E3Scene *any_scene = dynamic_cast<E3Scene *>(currentScene))              \
  {                                                                            \
    code                                                                       \
  }                                                                            \
  else if (H3Scene *any_scene = dynamic_cast<H3Scene *>(currentScene))         \
  {                                                                            \
    code                                                                       \
  }                                                                            \
  else if (H3LockScene *any_scene = dynamic_cast<H3LockScene *>(currentScene)) \
  {                                                                            \
    code                                                                       \
  }                                                                            \
  else if (H2EScene *any_scene = dynamic_cast<H2EScene *>(currentScene))       \
  {                                                                            \
    code                                                                       \
  }

#define FOR_ANY_SCENE_TYPED(currentScene, code)                                \
  if (E3Scene *any_scene = dynamic_cast<E3Scene *>(currentScene))              \
  {                                                                            \
    typedef E3Transformation any_scene_ttype;                                  \
    code                                                                       \
  }                                                                            \
  else if (H3Scene *any_scene = dynamic_cast<H3Scene *>(currentScene))         \
  {                                                                            \
    typedef H3Transformation any_scene_ttype;                                  \
    code                                                                       \
  }                                                                            \
  else if (H3LockScene *any_scene = dynamic_cast<H3LockScene *>(currentScene)) \
  {                                                                            \
    typedef H3LockedTransformation any_scene_ttype;                            \
    code                                                                       \
  }                                                                            \
  else if (H2EScene *any_scene = dynamic_cast<H2EScene *>(currentScene))       \
  {                                                                            \
    typedef H2ETransformation any_scene_ttype;                                 \
    code                                                                       \
  }
#endif