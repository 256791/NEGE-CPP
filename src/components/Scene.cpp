#include "Scene.hpp"

template <class TransformationType>
TypedScene<TransformationType>::TypedScene()
{
}
template <class TransformationType>
TypedScene<TransformationType>::~TypedScene()
{
}

template <class TransformationType>
void TypedScene<TransformationType>::addModel(std::shared_ptr<Model> model)
{
  guard.lock();
  models.push_back(model);
  guard.unlock();
}

template <class TransformationType>
void TypedScene<TransformationType>::addEntity(Entity<TransformationType> entity)
{
  guard.lock();
  entities.push_back(entity);
  guard.unlock();
}
template <class TransformationType>
void TypedScene<TransformationType>::setCamera(Camera<TransformationType>* camera)
{
  this->camera = camera; 
}

MAKE_TRANSFORMATION_TEMPLATE_TYPES(TypedScene)
