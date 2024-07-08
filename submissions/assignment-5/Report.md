# Group A #

Members:

- [Daniel Schade](https://github.com/D4ni3lSch4d3)
- [Ashutosh Solanki](https://github.com/AshIsAtWork)
- [Robin Cleve](https://github.com/cleveee)

# Code #

* Link to Pull Request: [Pull Request for Assignment 5](TODO!)
* Compiler: gcc 11.4.0

# Report #

---

## Task 1: Simulation of a membrane ##

---

## Task 2: Parallelization ##

---

## Task 3: Rayleigh-Taylor instability in 3D ##

## Task 4: Nano-scale flow simulation (Option A) ##

**1. Introduction**   
We decided to go for the nano-scale flow simulation instead for crystallization of argon, because it seemed to us a little bit more accessible. The big formula in task 5 is intimidating, even though you probably only have to copy it one-to-one from the worksheet. What could have been a bigger problem is the computation of the diffusion, because you have to apply special treatment for particles that crossed a periodic boundary which seemed to be involved at the first glance. Furthermore, we did not want to have anything more to do with periodic boundaries, because their implementation had already demanded a lot from us. Last but not least we think that a liquid traversing a little tube is more intuitive than observing a gas assuming different states.

**2. Implementation**   
The first problem we had to solve was how we make the walls fixed. Particles belonging to walls should not move, and no force should be exerted on them. Nevertheless, they should apply forces on other molecules that are not fixed. To distinguish between fixed and non-fixed particles, we added a new attribute `fixed` of type bool to the class `Particle`. A value of `true` indicates that the particle is fixed, a value of `false` that it should be treated normally. Before applying forces to a particle, altering its position or its velocity, we check first if the particle is fixed. If this is the case the particle will not be modified. For this we needed to add multiple if statements to our code. This is not pretty, and it will slow down our simulation a little bit even when no fixed particles a part of the simulation. A big advantage of this approach is that it is rather easy to implement, and does not require us to change the structure or logic of our program. In order to add walls or other structures to the simulation that do not move, we added to each element (cuboid, disc, sphere) the option in our xml-file to make it fixed.    

The next step was about implementing the new thermostat. We made the decision to introduce a new inheritance hierarchy by adding the abstract base class [Thermostat](../../src/moleculeSimulator/thermostats/Thermostat.h) from which the old thermostat, now renamed to [DefaultThermostat](../../src/moleculeSimulator/thermostats/DefaultThermostat.h) and the new thermostat, we called [FlowThermostat](../../src/moleculeSimulator/thermostats/FlowThermostat.h), inherit. We have gone for this approach multiple times in this project, and it always turned out to be a reasonable decision. It makes your code well-structured and extensible, because you can add other types of thermostats without making any major refactoring. You can specify which type of thermostat is used in the xml-file.   
The new thermostat resembles the old one with the important difference that you have to subtract the average velocity of the fluid in each step. This change achieves that the total velocity of the fluid is ignored. Now the temperature defines how much each particle's velocity deviates from the average. A high temperature means that the velocity distribution is heterogeneous, a low temperature means vice versa that it is homogeneous. This is useful for studying flows because the thing we are interested in is the relative velocity the particles have to each other rather than the total velocity of the fluid. For example, we want to know where in the tube the liquid moves relatively fast and where slower.   

To be able to study flows thoroughly, we must have access to more data than only the visualization in ParaView. Therefore, we implemented a profiler represented by the class [Statistics](../../src/moleculeSimulator/statistics/Statistics.h) that computes after every 10000 iterations a velocity and density profile of the liquid in observation and writes the collected data to a csv file to simplify further processing, for example, with Excel. For writing the csv files, we added a new writer [CSVWriter](../../src/fileHandling/outputWriter/CSVWriter/CSVWriter.h). Like suggested, we divided the x-axis into 50 bins and computed the average velocity in y-direction and the density of the fluid in each bin. After the simulation has finished the two profiles can be found in the directory `statistics` of your build folder.   

**3. Experiments**   

***3.1. First experiment***   
Time to test our setup and conduct a first experiment. We begin with the one given in the assignment. We got the following profiles:

<div style="display: flex; flex-wrap: wrap; justify-content: space-around">
    <img src="nano-scale-flow/standard-experiment/Velocity-Profile-Standard-Experiment.png" width = 45%></img> 
    <img src="nano-scale-flow/standard-experiment/Density-Profile-Standard-Experiment.png" width = 45%></img> 
</div>

* If you want to watch the video of the simulation (not interesting) click [here](nano-scale-flow/standard-experiment/Nano-Scale-Flow-Standard-Experiment.mp4).   

* If you want to run the simulation on your own machine, change into your build folder and run:
  ```bash
  OMP_NUM_THREADS=<#threads> ./MolSim -f ../input/assignment-5/task4-nano-scale-flow.xml -i xml
  ``` 
  ,where `<#threads>` is the number of threads you want to use. 
* The complete velocity profile you can find [here](nano-scale-flow/standard-experiment/Velocity-Profile-Standard-Experiment.csv).
* The complete density profile you can find [here](nano-scale-flow/standard-experiment/Density-Profile-Standard-Experiment.csv).

In the graphs, we have limited ourselves to visualizing the first 45 seconds of the simulation. This has multiple reasons. The first one is that including more data would decrease the clarity of the graphs. The second reason is that the average velocity of the fluid increases during the simulation so that we would have to extend y-axis of the graph to -200 if we wanted to show the whole plot resulting in the fact that series would overlap. Last but not least, the first 45 seconds provide you with everything you need for understanding what is happening. Observations stay qualitatively the same as the simulation goes on.   
As you might have noticed, we included the velocity profile at the beginning of the simulation (0s) but excluded the density at 0s in the graphs. This is again due to visualization purposes.   

What catches the eye immediately is that the downward velocity and density of the liquid are higher in the center of the tube than at the edges close to the walls. This difference in average velocity along the y-axis increases as the simulation progresses from which we can conclude that forces pointing downwards along the y-axis are higher for particles in the center than the ones close to the walls. Because of the gravitational force applied to each particle in negative y-direction is the same for all particles (energy is distributed evenly), there must happen some kind of energy transfer. We can think of two possibilities:    

1. Energy is transferred from particles close to the walls to particles in the center.
2. Energy is transferred from particles close to the walls to the particles at the walls and lost.

At the moment, we cannot tell what is exactly happening; maybe both presumptions are wrong. We need to conduct more experiments.    

***3.2. Increasing sigma of the particles within the walls***   
As starting point we want to study the effect when changing the Leonard-Jones parameter sigma of the particles belonging to the walls. As sigma controls mathematically the distance of the particles at which the Leonard-Jones potential exhibits its minimum and therefore the distance to which the force is repulsive we expect that increasing sigma will increase the gap between the wall and the fluid, because repulsive forces are getting stronger earlier and apply within a larger interval. We increased sigma from 1.1 to 3.0. Our simulation yields the following results for the first 45 seconds.    

<div style="display: flex; flex-wrap: wrap; justify-content: space-around">
    <img src="nano-scale-flow/larger-sigma-walls/Velocity-Profile-Larger-Sigma-Walls.png" width = 45%></img> 
    <img src="nano-scale-flow/larger-sigma-walls/Density-Profile-Larger-Sigma-Walls.png" width = 45%></img> 
</div>

* If you want to watch the video of the simulation, click [here](nano-scale-flow/larger-sigma-walls/Nano-Scale-Flow-Larger-Sigma-Walls.mp4).

* If you want to run the simulation on your own machine, change into your build folder and run:
  ```bash
  OMP_NUM_THREADS=<#threads> ./MolSim -f ../input/assignment-5/task4-nano-scale-flow-larger-sigma.xml -i xml
  ``` 
  ,where `<#threads>` is the number of threads you want to use.
* The velocity profile you can find [here](nano-scale-flow/larger-sigma-walls/Velocity-Profile-Larger-Sigma-Walls.csv).
* The density profile you can find [here](nano-scale-flow/larger-sigma-walls/Density-Profile-Larger-Sigma-Walls.csv).

We only conducted the first 45 seconds of the simulation, because, running it the whole 500 seconds would have taken a long time that we did not want to wait.    
As can be observed in the video, the gap between the walls and the liquid has increased significantly as we expected. The profiles, however, are not as we have thought. The velocity and density of the liquid are now distributed homogeneously. There is no (observable) difference anymore between particles close to and far from the walls. What is also interesting is the magnitude of the velocity of the liquid, which is now a lot higher compared to the previous experiment. The density is a little bit higher as well. As the gap between wall and liquid rises, the volume of the liquid shrinks and pressure rises, which could explain the increase in velocity.    

***3.3. Removing the walls and setting all boundary conditions to periodic***   
No we would like to study the scenario in which no walls are present at all and just observing the liquid flowing without any interactions with other objects. Therefore, we substituted the walls with periodic boundary conditions and adapted the domain size accordingly. Because there is no obstacle for the fluid we expect that the velocity and density of the fluid are distributed homogeneously.   

<div style="display: flex; flex-wrap: wrap; justify-content: space-around">
    <img src="nano-scale-flow/no-walls-all-boundaries-periodic/Velocity-Profile-No-Walls-All-Boundaries-Periodic.png" width = 45%></img> 
    <img src="nano-scale-flow/no-walls-all-boundaries-periodic/Density-Profile-No-Walls-All-Boundaries-Periodic.png" width = 45%></img> 
</div>

* If you want to watch the video of the simulation, click [here](nano-scale-flow/no-walls-all-boundaries-periodic/Nano-Scale-Flow-No-Walls-All-Boundaries-Periodic.mp4).

* If you want to run the simulation on your own machine, change into your build folder and run:
  ```bash
  OMP_NUM_THREADS=<#threads> ./MolSim -f ../input/assignment-5/task4-nano-scale-flow-all-periodic.xml -i xml
  ``` 
  ,where `<#threads>` is the number of threads you want to use.
* The velocity profile you can find [here](nano-scale-flow/no-walls-all-boundaries-periodic/Velocity-Profile-No-Walls-All-Boundaries-Periodic.csv).
* The density profile you can find [here](nano-scale-flow/no-walls-all-boundaries-periodic/Density-Profile-No-Walls-All-Boundaries-Periodic.csv).   

As the diagrams show, our assumptions are correct. The liquid exhibits a velocity downwards the y-axis which comes close to the experiment in which we increased sigma. Therefore, it is reasonable to assume that the total velocity of the liquid depends on the cross-section of the tube. The smaller the cross-section, the higher the velocity.   

***3.4 Observing Turbulences***    

Last but not least, we want to study what happens when obstacles are placed within the tube, disturbing the flow of the liquid. We placed a fixed sphere in the middle of the tube. We expect that the velocity in the middle where the sphere prevents the molecules from passing through decreases whereas the velocity at passages increases significantly. Running the simulation yields the following results:   

<div style="display: flex; flex-wrap: wrap; justify-content: space-around">
    <img src="nano-scale-flow/turbulence/Velocity-Profile-Turbulence.png" width = 45%></img> 
    <img src="nano-scale-flow/turbulence/Density-Profile-Turbulence.png" width = 45%></img> 
</div>

* If you want to watch the video of the simulation, click [here](nano-scale-flow/turbulence/Nano-Scale-Flow-Turbulence.mp4).

* If you want to run the simulation on your own machine, change into your build folder and run:
  ```bash
  OMP_NUM_THREADS=<#threads> ./MolSim -f ../input/assignment-5/task4-nano-scale-flow-turbulences.xml -i xml
  ``` 
  ,where `<#threads>` is the number of threads you want to use.
* The velocity profile you can find [here](nano-scale-flow/no-walls-all-boundaries-periodic/Velocity-Profile-No-Walls-All-Boundaries-Periodic.csv).
* The density profile you can find [here](nano-scale-flow/no-walls-all-boundaries-periodic/Density-Profile-No-Walls-All-Boundaries-Periodic.csv). 

The profiles look as expected. The velocity in y-direction is smaller where the obstacle blocks the flow. Which is interesting is that the overall velocity of the fluid is pretty low compared to all previous simulations which indicates that turbulences occurred, meaning that particles are flowing not straight down but also in other directions. To study this further, computing velocity profiles on the other two axes would give valuable insights.   

***3.5 Conclusion***   
We only did cover a tiny fraction of all influences you could possibly study. The results of some simulations turned out as expected, but some other effects like the velocity distribution in the first experiment we cannot explain properly so far. We only can presume what might be reasons for this behaviour. Nevertheless, it was fascinating to test various scenarios, compare them and think of a possible explanation. The main reason why we did not carry out more experiments is that the calculation of each simulation took a very long time. We executed it on our own machine (with only four cores available), because the Linux cluster is always crowded and therefore did not help much to get results faster.   





