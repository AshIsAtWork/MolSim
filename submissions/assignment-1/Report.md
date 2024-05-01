# Group A #
Members:
- [Daniel Schade](https://github.com/D4ni3lSch4d3)
- [Ashutosh Solanki](https://github.com/AshIsAtWork)
- [Robin Cleve](https://github.com/cleveee)

# Code #
* Link to Pull Request: [Pull Request for Assignment 1](https://github.com/AshIsAtWork/MolSim/pull/2)
* Compiler: gcc 11.4.0

# Report #

## Task 1: First Steps ##

* This task was all about installing some programs and doing some initial preparation for a good coding environment. 
  Installing ParaView was not that easy as we expected it to be. We had to try different versions, because some versions refused to run on our linux system (Ubuntu). In the end we succeeded to run version 5.9.
* Installing doxygen was quickly done over the command line. 
* In the first few days, when we were not yet so well-rehearsed as a team, each of us set up a repository by its own. On Monday was our first meeting at university, where we agreed on working with Ashutosh's repository. He has had set up already clang-tidy to assure us writing clean code being one example of using tools to increase code quality.  

## Task 2: First Pull Request ##

* Adjusting the top-level README.md to the requirements was quickly done. We added a short instruction how to build and run our program. To this point some of us have not been aware that these steps are necessary to ensure that everyone is able to work with our program. Working on a bigger software projects is quite different from implementing small programs, having only to run on one's own machine. 
* On Thursday we created our first pull request. Unfortunately we had just been able to add Fabio, because the other two supervisors did not accept our request to become contributor of our repository in time. 

## Task 3: Completion of the program frame ##

**1. Implementing the physical equations**
* Before we started with the implementation, we first got an overview of all the functions that the basic program already provides. It would have been very annoying to realise after implementing everything on our own that the implementation could have been significantly simplified by using an already existing function. For example, we came across the L2Norm template, which takes care of calculating the Euclidean distance between two particles. We noted as well the various overloaded operators, greatly simplifying vector calculations. With that knowledge the implementation of the formulas, which were fortunately already given in the lecture slides, could be realised quite elegant. 
* Unfortunately, we messed something up while adopting the formulas which resulted in some really strange behaviour as we tried to simulate and visualize the output for Halley's comet as first test. After some hours of annoying bug finding, we found the little mistake and received reasonable results.
**2. Visualization using ParaView**
* The visualization of our results has proven to be very helpful to see, if everything works correctly. Calculating everything by hand and compare the results is already for just a few steps an incredible effort. In order to create an animation of the output with ParaView, we first had to change the output format from .xyz to .vtu, as apparently ParaView works only properly with this file format. As most of his was already implemented, the changeover was quickly done.
* As mentioned above we were kind of surprised when seeing the simulation of our erroneous program. Instead of circling around each other, all particles moved away from one another, which, with the imagination of our planet system in mind, could not be correct. After we had corrected the bug, visualization was as expected. The two planets Earth and Jupiter circled around the almost static sun, the comet surrounding the sun on a big ellipse. That corresponded to our intuition. 
**3. Parsing arguments of the command line**
* As last step of this task we implemented the parsing of the arguments delta_t and t_end from the command line. We searched the internet for some convenient way to do this in C++ and found the boost library which offers some functions especially designed for this task. Without that library the argument parsing would have become much more difficult, because we knew already that argument parsing is really error-prone. 


## Task 4 ##
**1. Identification of the celestial bodies**
* As already stated in task 3, we used this task to sanitize our implementation of the formulas, showing us some issues. After correcting this bug the visualization using ParaView was as expected. As parameters for the simulation we used delta_t = 0.014 and end_t = 1000 as suggested in the task description. Of course, you could use smaller time steps to increase the accuracy of the calculation, but we concluded that this does not improve the visual output of ParaView significantly, so we stuck with the default ones. Furthermore, using smaller time steps results in longer running times and more data files, which is not worth the improvement you get from it. Having produced our vtk- files we tried to deduce from the visualization, which particle represents which celestial body  :
  * **Sun**: Identifying the sun was the easiest task. It had to be the object which had by far the most mass and which hardly moved at all in the animation. Furthermore, all other objects circled around it. 
  * **Haley's comet**: Identifying Haley's comet was not that difficult either. According to our intuition it had to be the object that has the smallest mass and that's trajectory does not resemble one which can be observed for planets, meaning, it resembles more an ellipse than a circle.
  * **Earth and Jupiter**: Judging by their orbit's, both of the other objects must be planets in our solar system. We searched the Internet for the planets with appropriate masses and distances from the sun and were able to determine that the object that rotates in relatively tight circles around the sun must be our Earth and the more distant object must be Jupiter.
**2. Creating and exporting a beautiful visualization**
* Creating a nice looking video with ParaView was not that easy. First of all we had to play around a little to get used to this program and find some useful features. We colored all particles corresponding to their real counterparts in reality, tried to model the trajectories, included a scale and some legend. Last but not least we used a black background evoking the feeling of actual being in space. 
* We also tried to track the particles in order to create their orbits dynamically and keep them up to date. Static trajectories suffer from the drawback that the actual orbits are not perfectly stable but differ in each step, so the static model becomes more and more inaccurate during the animation. Unluckily the function provided by ParaView to do this (temporal particles to pathlines) did not work properly, so we had to give up this idea. 



## Task 5 ##
**1. Storing the particles**
* For our implementation of storing the particles we used the predefined class std::vector from the standard library. The reason for this is that internally all objects are stored consecutive in memory making the access cache efficient especially when iterating over them. To secure a performant way of iterating is really crucial to guarantee a fast simulation. Furthermore, std::vector is in contrast to std::array dynamic. This is necessary, because we can't tell before compilation, how many particles have to be stored. Additionally, we can add more particles during the simulation without worrying of going out of storage. 
* To keep iteration as simple as possible we provided an iterator making the particle container suitable for iterating over pairs of particles, by simply nesting two for loops as shown in the method calculateF() of the class Simulator. 
* In order to be able to extend our program easily in the future we had the following ideas:
  * **Force calculation**
  * To adapt different meanings of a force between two particles in the future, we introduced the new interface Force, which is a completely abstract class. All meanings of forces coming up in the future should extend this interface by implementing its abstract method compute(Particle target, particle source). The class Gravity is an example for a force acting between two particles.
  * **File handling**
* To provide an efficient way of handling multiple methods of IO, we introduced the class FileHandler which does nothing else than encapsulating all methods, which are currently there for in- and output. All means of IO coming up in the future can be added to this class.
* We try to make use of references where we can, especially when handling bigger objects to avoid some unnecessary copying.
* Additionally, we introduced a new class called Simulator to keep the main file as clean as possible and to give our code a more understandable structure. 
* Doxygen is a nice tool to automatically generate a documentation of your code. To create a documentation while building the project just follow the instructions in the README file. The documentation can then be found inside build/docs/html/index.html.