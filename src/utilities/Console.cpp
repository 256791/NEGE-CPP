#include "Console.hpp"
#include "../parsers/ObjParser.hpp"
#include "../parsers/SceneParser.hpp"

Console::Console(Engine *engine, std::istream &in, std::ostream &out)
    : in(in), out(out), engine(engine)
{
  started = false;
  running = false;
  engine->events.push_back(this);
}

void Console::start()
{
  if (!started)
  {
    started = true;
    running = true;
    self = std::thread(&Console::run, this);
  }
}

void Console::join()
{
  if (started)
  {
    started = false;
    self.join();
  }
}

void Console::stop()
{
  running = false;
  join();
}

bool Console::handle(Engine *engine)
{
  if (eventQueueGuard.try_lock())
  {
    while (!eventQueue.empty())
    {
      eventQueue.front()(engine);
      eventQueue.pop();
    }
    eventQueueGuard.unlock();
  }
  return false;
};

void Console::run()
{
  while (running)
  {
    std::string buff;
    in >> buff;
    if (buff == "entities")
    {
      if (engine->currentScene == nullptr)
        out << "No scene\n";
      else
      {
        FOR_ANY_SCENE(engine->currentScene, {
          for (auto entity : any_scene->entities)
            out << entity.getName() << '\n';
        })
      }
    }
    if (buff == "models")
    {
      if (engine->currentScene == nullptr)
        out << "No scene\n";
      else
      {
        FOR_ANY_SCENE(engine->currentScene, {
          for (auto model : any_scene->models)
            out << model->getName() << '\n';
        })
      }
    }
    else if (buff == "spawn")
    {
      if (engine->currentScene == nullptr)
        out << "No scene\n";
      else
      {
        in >> buff;
        FOR_ANY_SCENE_TYPED(engine->currentScene, {
          for (auto model : any_scene->models)
            if (model->getName() == buff)
            {
              any_scene->addEntity(Entity<any_scene_ttype>(model));
            }
        })
      }
    }
    else if (buff == "load")
    {
      in >> buff;
      auto pos = buff.rfind('.');
      if (pos != std::string::npos)
      {
        std::string ext = buff.substr(pos + 1);
        if (ext == "obj")
        {
          try
          {
            ObjParser p;
            p.parse(buff);
            std::vector<std::shared_ptr<Model>> models = p.getModels();
            for (std::shared_ptr<Model> model : models)
            {
              out << "Loaded: " << model->getName() << '\n';
              FOR_ANY_SCENE(engine->currentScene, {
                any_scene->addModel(model);
              })
            }
          }
          catch (...)
          {
            out << "Could not load model\n";
          }
        }
        else if (ext == "scn")
        {
          try
          {
            Scene *scene = SceneParser::loadScene(buff);
            eventQueueGuard.lock();
            eventQueue.push(std::function<void(Engine * engine)>(
                [scene](Engine *engine) -> void
                { engine->changeScene(scene); }));
            eventQueueGuard.unlock();
          }
          catch (...)
          {
            out << "Could not load scene\n";
          }
        }
        else
        {
          out << "Unknown extension\n";
        }
      }
      else
      {
        out << "Unknown extension\n";
      }
    }
    else if (buff == "end" || buff == "exit")
    {
      out << "Terminating...\n";
      eventQueueGuard.lock();
      eventQueue.push(std::function<void(Engine * engine)>(
          [](Engine *engine) -> void
          { glfwSetWindowShouldClose(engine->getWindow(), GLFW_TRUE); }));
      eventQueueGuard.unlock();
      running = false;
    }
    else
    {
      std::cout << "Not recognized\n";
    }
  }
}
