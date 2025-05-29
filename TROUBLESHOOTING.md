# Diary

1. 2025.05.19 Zero weight problem  

```c++

src/Model.h:134
void SetVertexBoneData(Vertex &vertex, int boneID, float weight) {
  if (weight == 0.0f)
    return;

  for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
    if (vertex.m_BoneIDs[i] < 0) {
      vertex.m_Weights[i] = weight;
      vertex.m_BoneIDs[i] = boneID;
      break;
    }
  }
}
```

if weight == 0.0f is not guarded, then it will create noise, appearing 
to have weight with very fractional weight


2. 2025.05.19 Use After Free (std::vector resizing..)  
src/Types/Animation/Animator.h:22  
Need to be cautious the functional difference between resize and reserve

3. 2025.05.22 Ragdolls...  
Not necessarily troubleshooting but it's just too hard to create ragdolls  
Its so hard without visual assistance since it's all by code  

4. 2025.05.28 Sampling textures in Vertex Shader
```bash
UNSUPPORTED (log once): POSSIBLE ISSUE: unit 0 GLD_TEXTURE_INDEX_2D is unloadable and bound to sampler type (Float) - using zero texture because texture unloadable
```

Trying to investigate why the driver won't recognize the textures I have sent  
Perhaps macOS issues..? Reddit, github, stackoverflow, and all communities don't have clear answers  

Ok FIXED  
It was with importing EXR, will comprehend later  
