#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "../Event.hpp"
#include <vector>
#include <utility>

class Animation : public Event
{
};

template <class TransformationType>
class TransformationAnimation : public Animation
{
public:
  TransformationAnimation(std::shared_ptr<TransformationType> transformation);
  void addKeyFrame(TransformationType state, double timeDelta);
  bool handle(Engine *engine) override;

private:
  std::shared_ptr<TransformationType> transformation;
  TransformationType startFrame;
  std::vector<std::pair<TransformationType, double>> keyFrames;
  int current_keyframe;
  double last;
};

// template <class TransformationType>
// class CameraAnimation : public TransformationAnimation
// {

// };
#endif