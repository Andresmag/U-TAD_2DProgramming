# Master's Degree in Video game programming at U-Tad
## 2D Programming

Practices for the subject 2D Programming.

* Practice 1: Drawing of graphical primitives. Trigonometric calculations.
* Practice 2: Textures and blending modes.
* Practice 3: Text rendering.
* Practice 4: Sprites and animation.
* Practice 5: Collisions.
* Practice 6: The World. Camera and scroll control.
* Practice 7: Loading maps.

Every practice is an improvement from the last one so a lot of the code is reused.
But you should look at it closely because sometimes although it's really similar it has a little change.

The base library to develop the practices is LiteGFX an easy to use 2D and 3D graphics library based on OpenGL's fixed pipeline.
You can find more about this library at its creator's [repository](https://github.com/JaviCervera/litelibs) 

### Instructions to compile and run the code
* Folder structure must be:
  * lib
  * data (when necessary -all except practice 1-)
  * project
  * src
* Lib folder outside the practices contains the glfw and litegfx libraries so it must be **included and combined** within the lib folder in **every practice** (I've uploaded it this way not to upload the libraries 7 times)
* To run the code, be sure to **compile it for x86**. Any other way it will fail.

*In the practice 5, the per pixels collision between the bee and the circle is not working fine but after
checking the code several times I couldn't find the error.*
