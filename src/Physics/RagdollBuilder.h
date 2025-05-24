#pragma once

#include "Model.h"
#include "Physics.h"

class RagdollBuilder {
public:
  static RagdollSettings *Create(const Model &model, const std::string &ragdollJsonPath);
};
