#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <memory>

class Animation {
public:
  Animation() = default;
  void        init(std::string name, std::string path);
  aiNodeAnim *find_node_anim(const std::string &name);
  float       calc_animation_time_ticks(float time_seconds, unsigned int animation_index);

  const std::string &get_name() {
    return _name;
  }
  const float &get_duration() {
    return _duration;
  }
  const float &get_ticks_per_second() {
    return _ticks_per_second;
  }

private:
  std::string                                   _name;
  float                                         _duration;
  float                                         _ticks_per_second;
  std::unordered_map<std::string, aiNodeAnim *> _node_anim_map;
  std::map<const char *, unsigned int>          _node_mapping;

  std::unique_ptr<Assimp::Importer> _importer;
  const aiScene                    *_scene;
};
