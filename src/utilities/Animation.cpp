#include "Animation.hpp"

template <class TransformationType>
TransformationAnimation<TransformationType>::TransformationAnimation(std::shared_ptr<TransformationType> transformation)
    : transformation(transformation), current_keyframe(-1) {}

template <class TransformationType>
void TransformationAnimation<TransformationType>::addKeyFrame(TransformationType state, double timeDelta)
{ 
  keyFrames.push_back(std::make_pair(state,timeDelta));
}

template <class TransformationType>
bool TransformationAnimation<TransformationType>::handle(Engine *engine)
{
  if(current_keyframe == -1){
    current_keyframe = 0;
  }
  engine->getCurrentFrame();
}

MAKE_TRANSFORMATION_TEMPLATE_TYPES(TransformationAnimation)