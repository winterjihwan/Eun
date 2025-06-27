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

5. 2025.05.30 UI  
I tried everything, I debugged with xcode metal debugger, logged out all the uniforms and attributes  
that were supposed to go to the shaders  
everything was clear and good but nothing rendered on the screen  
turned out that the triangle vertices order was clockwise  

6. 2025.05.30 UI  
Reminder to myself  
Please set uniforms after binding shader PLEASE  

7. 2025.05.31  
Previously used  
```c++
float *_pixel_data;
```
To store pixel data, which will be used later when uploading to GPU  
However seldomly segfaults  
Not because the vector resized, had to copy ExrTexture
```c++
std::unique_ptr<float[]> _pixel_data;
```
So I used Rust-like solution to Box it out and OWN the address  

```c++
unsigned int             _texture_id;

if (_texture_id != 0) {
    std::cerr << "ExrTexture::upload_to_gpu, already has texture id" << std::endl;
    assert(0);
}
```
_texture_id needed to be zero initialized, or some dump value might be placed   

8. 2025.06.04
Was planning to do ECS  
but I decided to do it in my next game engine  
I found it not really that useful for this game + needs tons of digging up  
which I am not hesistant BUT I don't have infinite resources  
Hence I chose general struct of Entity + on update callbacks  
Most of my so called 'Entities' differ in behavior, and not in data  
So this might work better  

9. 2025.06.05
Had troubles re exporting models and animations from blender  
Spend 10 hours, and I got a solution (not complete tho)  
1. import  
2. Select Armature then scale -> 1.0  
3. Select all then Apply all transforms  
4. Select Armature Apply Visual Transform  
5. Pose mode -> click A to select all bones -> Apply visual transform to pose  
6. Select all then dae export  
7. Global Orientation Apply. Forward axis: -Z, Up. Axis: Y  

10. 2025.06.22
While Framebufferifying, I had accidentally put viewport to 0 hence the attachments to the framebuffers  
were actually not being created. That's why framebuffer itself was faulty  

11. 2025.06.26
Noticed the opposite side was not being renderered  
Turns out if its completely 2D and I did not draw the opposite side,  
it will be culled out entirely  

12. 2025.06.27
Had Sanitize error for JPH when I tried to register static mesh for Model  
There were two errors  
1. Assimp load import flags  
    JoinIdenticalVertices, SplitLargeMeshes would mess around the vertices that needs special treatment  
    when proccesing in the CPU side which is too buggy  
2. Quaternion not being normalized  
