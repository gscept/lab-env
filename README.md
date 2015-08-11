# lab-env
C++11 based application base for developing apps.

# Requirements
  CMake 3+
  Compiler with support for lambda functions (C++11), such as VS 2013.
  
# Instructions
  Put your laboration in the folder titled 'projects'. It must be accompanied with its own CMakeLists.txt which is used to generate a project. Then run CMake in the root of the project, there should be a target in your project matching the name of your lab.
  
# Usage
  The lab environment contains two classes, Window and App. Override the App class for your project to implement your own update loop. The Window class is used to open a window and connect input handlers to it. This is done by assigning a lambda function to a callback matching the input you require.
