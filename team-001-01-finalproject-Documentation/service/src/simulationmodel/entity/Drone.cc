#define _USE_MATH_DEFINES
#include "Drone.h"

#include <cmath>
#include <limits>
#include "math/vector3.h"

#include "AstarStrategy.h"
#include "BeelineStrategy.h"
#include "BfsStrategy.h"
#include "DfsStrategy.h"
#include "DijkstraStrategy.h"
#include "JumpDecorator.h"
#include "Package.h"
#include "SimulationModel.h"
#include "SpinDecorator.h"

#include "DataCollector.h"

Drone::Drone(const JsonObject& obj) : IEntity(obj) { available = true; }

Drone::~Drone() {
  if (toPackage) delete toPackage;
  if (toFinalDestination) delete toFinalDestination;
}

void Drone::getNextDelivery() {
  if (model && model->scheduledDeliveries.size() > 0) {
    package = model->scheduledDeliveries.front();
    model->scheduledDeliveries.pop_front();

    if (package) {
      available = false;
      pickedUp = false;
      startPosition = {};
      startPosition = position;  // Store the current position at pickup
      totalDistance = 0.0;  // Reset the distance traveled

      Vector3 packagePosition = package->getPosition();
      Vector3 finalDestination = package->getDestination();

      toPackage = new BeelineStrategy(position, packagePosition);

      std::string strat = package->getStrategyName();
      std::cout << " In getNextDelivery = >Strategy: " << strat << std::endl;
      if (strat == "astar") {
        toFinalDestination =
            new JumpDecorator(new JumpDecorator(new AstarStrategy(
                packagePosition, finalDestination, model->getGraph())));
      } else if (strat == "dfs") {
        toFinalDestination =
            new JumpDecorator(new JumpDecorator(new DfsStrategy(
                packagePosition, finalDestination, model->getGraph())));
      } else if (strat == "bfs") {
        toFinalDestination =
            new JumpDecorator(new JumpDecorator(new BfsStrategy(
                packagePosition, finalDestination, model->getGraph())));
      } else if (strat == "dijkstra") {
        toFinalDestination =
            new JumpDecorator(new JumpDecorator(new DijkstraStrategy(
                packagePosition, finalDestination, model->getGraph())));
      } else {
        toFinalDestination =
            new BeelineStrategy(packagePosition, finalDestination);
      }
    }
  }
}

void Drone::update(double dt) {
  // Store the previous position before any movement
  Vector3 previousPosition = position;
  if (available) {
    getNextDelivery();
  }

  if (toPackage) {
    toPackage->move(this, dt);  // Move towards the package
    checkDirectionChange();  // Check for direction changes

    // After moving, check if the drone has reached the package
    if (toPackage->isCompleted()) {
      std::string message = getName() + " picked up: " + package->getName();
      notifyObservers(message);
      pickUpPackage(*package);  // Pickup the package
      delete toPackage;
      toPackage = nullptr;
      pickedUp = true;
      // Initialize startPosition at the point of pickup
      startPosition = position;
      totalDistance = 0.0;  // Reset when package is picked up
      routeTimeElapsed = 0.0;
    }
  } else if (toFinalDestination) {
    toFinalDestination->move(this, dt);  // Move towards the final destination
    checkDirectionChange();  // Check for direction changes

    // Update the totalDistance with the distance moved in this time step
    if (package && pickedUp) {
      // Only start tracking distance after the package has been picked up
      totalDistance += sqrt(pow(position.x - previousPosition.x, 2) +
                            pow(position.y - previousPosition.y, 2) +
                            pow(position.z - previousPosition.z, 2));
      package->setPosition(position);
      package->setDirection(direction);
      routeTimeElapsed += dt;
    }

    // Check if the final destination has been reached
    if (toFinalDestination->isCompleted()) {
      std::string message = getName() + " dropped off: " + package->getName();
      notifyObservers(message);

      model->deliveryCounts[package->getStrategyName()]++;
      dropOffPackage(*package);  // Drop off the package
      delete toFinalDestination;

      toFinalDestination = nullptr;
      package->handOff();
      package = nullptr;
      available = true;
      pickedUp = false;
    }
  }
}

void Drone::pickUpPackage(const IEntity& entity) {
    const Package* package = dynamic_cast<const Package*>(&entity);
    if (package) {
        routeData.push_back(package->getStrategyName());
        std::string start = std::to_string(position.x) + ":"
            + std::to_string(position.y) + ":" + std::to_string(position.z);
        std::string end = std::to_string(package->getDestination().x) + ":"
            + std::to_string(package->getDestination().y) + ":"
            + std::to_string(package->getDestination().z);
        routeData.push_back(start);
        routeData.push_back(end);
        routeTimeElapsed = 0.0;
    } else {
        std::cerr << "Error: IEntity provided is not a Package!" << std::endl;
    }
}

void Drone::dropOffPackage(const IEntity& entity) {
    const Package* package = dynamic_cast<const Package*>(&entity);
    if (package) {
        DataCollector *dc = DataCollector::GetInstance();

        // Log distance and time
        routeData.push_back(std::to_string(totalDistance));
        routeData.push_back(std::to_string(routeTimeElapsed));

        // Include turn counts
        routeData.push_back(std::to_string(rightTurnCount));
        routeData.push_back(std::to_string(leftTurnCount));

        // Collect all data
        dc->collectData(routeData);
        routeData.clear();

        // Reset turn counts and time
        rightTurnCount = 0;
        leftTurnCount = 0;
    } else {
        std::cerr << "Error: IEntity provided is not a Package!" << std::endl;
    }
}

double Drone::angleBetweenVectors(const Vector3& v1, const Vector3& v2) {
    double dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    double magV1 = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
    double magV2 = sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
    double angle = acos(dot / (magV1 * magV2));
    return angle * (180.0 / M_PI);  // convert radians to degrees
}

void Drone::checkDirectionChange() {
    Vector3 currentDirection = direction;  // Current facing direction vector
    double angle = angleBetweenVectors(previousDirection, currentDirection);

    if (angle >= 10) {  // Threshold angle to count as a significant turn
        if (currentDirection.cross(previousDirection).z > 0) {
            rightTurnCount++;
        } else {
            leftTurnCount++;
        }
    }
    previousDirection = currentDirection;  // Update for the next cycle
}
