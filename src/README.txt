Alright so I will go over a bit of the architecture of this path tracer:
- I will rarely use virtual functions to preserve memory and speed. One of the side effects from this is that hittable types must be supported explicitly by the engine, and those types will probably only be triangles. I could go with a hybrid approach but I will work on that later
- Reorganize how triangles and primitives are used

TODO:
- Read more about ray intersection algorithms, and also add support for others
- Refractor code until most things are in source files instead of header files and the overall code looks neater
- Support for movable perspective camera
- Use typedefing or using to get rid of the glm namespace from my code
- Restructure code
- Improve speed