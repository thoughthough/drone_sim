**Section:** 001 
**Team:** 01


* Zachary Ross zachary.ross.w@gmail.com




# Overview 
Our program was designed to simulate the delivery of packages on the University of Minnesota campus. The model includes  a frontend that allows users to, through a browser, schedule deliveries and visualize them in real time by using 3D models of the campus as well as the multiple entities that exist in the model. These entities include drones which deliver the packages, robots which receive the deliveries, humans, a helicopter, and the packages themselves.
The front end communicates with a backend that calculates the updated state of the model, which is then sent back to the frontend to be displayed to the user. 
Internally, the map of the campus is represented as a graph, so in order to complete the delivery, drones are assigned one four different path finding algorithms that they will use to navigate to their destination. During a session, data about each delivery is collected and stored in a csv format, and from which graphs are automatically generated in order to better analyze the data.


# Running The Simulation
To run the simulation, first navigate to the directory csci-3081w-s24/team-001-01-finalproject and then run the following commands.
Run the commands 

```
make clean
make -j
make run
```
You can then navigate to  the url http://127.0.0.1:8081/ in a browser and the simulation should begin once connected.
Note: I added a change to the Makefile that should open a webpage automatically

On start up four deliveries are scheduled, and 4 drones, each using a different strategy will deliver packages to a specific robot. When a drone reaches a robot,the robot teleports to a new location and a delivery of the same package is scheduled again to that robot. This means a drone will follow a particular robot around the map delivering the same package until the simulation is stopped. This allows the the simulation to run indefinitley without using extensive memory creating new robots and packages. 

The simulation can be ended by clicking the "Stop Simulation" button in the UI. Upon close, the CSV with the collected data along with various graphs related to the completed deliveries will be automatically generated in the logs folder.


# The Feature
Our feature adds the ability to collect and store data related to each delivery in a global database which is then written to a CSV file. 
This is interesting because it allows you to apply various statistical techniques to analyze the behaviour and performance of different aspects of the model, in our case how each search strategy performed. This adds to the previous work you miss out on a lot of useful information if you have way of collecting data while the simulation is running.

In order to maintain a global database, the DataCollector class needs to cache the data for each completed delivery . Each drone accessing a different instance of the DataCollector class adds complicated logic to the program.  In addition, this approach does not scale very well as each drone using the database needs to create a new instance of the Database class. We chose the Singleton design pattern to implement this feature as itt addresses these issues. Only  one instance of the DataCollector class is created for which is globally accessible. This is resource efficient, allows all data to be cached in one instance, makes it simpler to access, and makes the logic around writing the data more streamlined.

This was accomplished by adding a new class, dataCollector, which follows the Singleton design pattern. This class is only instantiated once, and that instance can be accessed by all other classes. During the process of each delivery, each drone maintains a vector of relevant data points, and upon delivery, that data is passed to the dataCollector instance which maintains a global related to every completed delivery.  The data collected is strategy type, starting location, ending location, distance traveled, right turns taken, left turns taken, and elapsed time. These are delivery specific so the elapsed time and distance traveled is from package pickup to package drop off.

This project centers around the collection of data through a Singleton design pattern called dataCollector. This implementation takes strings of data from a drone and stores it in a vector. On pressing the Stop Button the dataCollector creates a csv file containing the vector data. The data is then analyzed automatically and several graphs are generated from it. The data and the processed data focus on efficiencies that could be achieved while the drone has the package. To this end we have modified the simulation so that 4 drones are issued randomized deliveries. Each drone specializes in one of four strategies, A*, BFS, DFS, and Dijkstra. We also modified the Robot so that it teleports to the new location after it receives its package. This allows the simulation to be run automatically for an extended period of time without crashing like it otherwise would from having too many robots. The simulation’s Makefile has also been changed so that it launches or prompts the user to open the website hosting the simulation. 



# Using the new feature
Nothing is needed on the users end to use the new feature, as the data collection is done automatically. The generated images and csv file will be located in the logs folder can be viewed after a session ends.

# Instructions for How To Use Docker

Your can run download and run the docker image using the following commands:
```
docker pull thoughthough/drone_sim
docker build -t drone_sim
docker run -p 8081:8081 drone_sim
```
Then go to url http://127.0.0.1:8081/ on any browser and the program will start running.

# Sprint Retrospective
We worked well together making timely and incremental progress on this sprint.We are very happy with the demo product and we believe we hit all sprint task goals to satisfaction. We could work more on understanding the definition of done for specific features, a better (earlier) understanding of the user’s needs would have been useful. We should have scheduled Daily Scrums to coordinate on tasks and better understand impediments. In the future scheduling regularly would have improved coordination and overall output quality.

 # Links
 [DockerHub](https://hub.docker.com/r/thom7918/team-001-final-project)
 
 [Youtube Presentation Link](https://youtu.be/kcOyvIg9XmA)
 
[Github Project](https://github.umn.edu/umn-csci-3081-s24/team-001-01-finalproject)



