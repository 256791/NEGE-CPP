#include "Entity.hpp"

template <class TransformationType>
Entity<TransformationType>::Entity(std::shared_ptr<Model> model) : model(model)
{
  transformation = new TransformationType();
}

template <class TransformationType>
Entity<TransformationType>::~Entity(){
  // delete transformation;
}

template <class TransformationType>
void Entity<TransformationType>::draw(Material::Location materialLocation)
{
  model->draw(materialLocation);
}

MAKE_TRANSFORMATION_TEMPLATE_TYPES(Entity)
