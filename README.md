# RubicsCubeOpenGL

This is an OPENGL usage in c++ demonstrating a rubics cube(n X n) and animating its rotations along particular axises.

The usage is really simple

**$ g++ rubics.cpp cube.hpp solver.hpp animater.hpp -lglut -lGLU -lGL -lm -fPIC -I /usr/include/eigen3 -o rubics**


It has got an **eigen** dependency as well as obviously **glut** for openGL

Project done in ubuntu **overnight**... Just felt like doing it.

The original purpose was to create a solver too...  using a naive breadth first search

Soon realized my naiveness, as for getting even to the 4<sup>th</sup> depth in the search it is taking ages.

The identical cube function needs update over the 24 looks of a same representation. The code creates 6 cube representations in the variable **cube_6**. You can just take identicality with the 4 rotations over X axis for each of the cube transitions. This is preety easy to do and will make the  identical function between two cubes perfect.  
Do not feel like doing it anymore now...

After doing 

**$ ./rubics**

use the number keys to observe some rotation animations
