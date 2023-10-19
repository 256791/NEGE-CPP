#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <memory>
#include "Model.hpp"
#include "Transformation.hpp"

class BaseEntity{

};

template <class TransformationType>
class Entity
{
public:
  Entity(std::shared_ptr<Model> model);
  ~Entity();

  void draw(Material::Location materialLocation);
  const std::string getName() const { return model->getName(); }

  TransformationType *transformation;
  int id;

protected:
  std::shared_ptr<Model> model;

};

#endif