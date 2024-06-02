# Project Overview
In this project, the goal is to automate, where possible, a hydroponic greenhouse in an urban context. Due to timing and resource constraints, the scope has been appropriately relaxed, focusing exclusively on the preparation plant for the germination phase (which will be detailed in Chapter 3) and the germination phase itself for the cultivation and production of lettuce.

# Objectives
Despite being downscaled, the objectives of this project are:
1. To automate the preparation phase for germination, thereby requiring only an essential number of designated workers.
2. To calibrate a PI controller to automatically adjust the temperature value of the germination room, where the plants will spend the first phase of their growth cycle.

# Process Phases
## Preparation for the Germination Phase
The preparation plant for the germination phase will prepare the pots with the substrate, plant the lettuce seeds inside them, cover them, and irrigate them with a suitable nutrient solution.

## Germination Phase
The actual growth phase of the lettuce seeds begins inside the germination room. To allow controlled growth in a real case, it would be necessary to monitor the values of temperature, humidity, light, and ventilation inside the room; however, as already discussed in section 2.1, our system will reduce to a simple model with a differential equation for heat exchange from one room to another. At the head of our system, a PI controller will be connected to effectively regulate only the temperature value in case of deviation from the optimal range.

# Tools and Techniques Used
## Preparation Plant for the Germination Phase
For the implementation of the control logic of the individual machinery, the SFC language was used, with some additions of elements in Ladder (for some specific actions/transitions) and Structured Text (specifically for the use of SFCInit and SFCPause tags).
For the implementation of the simulation logic, only the Ladder language was used.
CODESYS (V3.5 SP16 Patch 4) was chosen as the editor and environment for simulations.
The machinery drawings were created by us on paper and then digitized using Adobe Illustrator.

## Control of Temperature Inside the Germination Room
For the implementation of a solution to the thermal exchange model control problem, MATLAB was used, from which we could easily derive all the diagrams. The solution was then schematized on Simulink for simulation purposes.
