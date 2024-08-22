#ifndef DRONE_H_
#define DRONE_H_

#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include <cmath>


class Package;

// Represents a drone in a physical system.
// Drones move using euler integration based on a specified
// velocity and direction.
/**
 * @class Drone
 * @brief Represents a drone in a physical system. Drones move using euler
 * integration based on a specified velocity and direction.
 */
class Drone : public IEntity {
 public:
  /**
   * @brief Drones are created with a name
   * @param obj JSON object containing the drone's information
   */
  Drone(const JsonObject& obj);

  /**
   * @brief Destructor
   */
  ~Drone();

  /**
   * @brief Gets the next delivery in the scheduler
   */
  void getNextDelivery();

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   */
  void update(double dt);

  /**
   * @brief Removing the copy constructor operator
   * so that drones cannot be copied.
   */
  Drone(const Drone& drone) = delete;

  /**
   * @brief Removing the assignment operator
   * so that drones cannot be copied.
   */
  Drone& operator=(const Drone& drone) = delete;

  /**
   * @brief Picks up a package, starting the delivery process.
   * @param package Reference to the package entity to be picked up.
   */
  void pickUpPackage(const IEntity& package);

  /**
   * @brief Drops off a package at the destination.
   * @param package Reference to the package entity to be dropped off.
   */
  void dropOffPackage(const IEntity& package);

  /**
   * @brief Checks and handles changes in the drone's direction during flight.
   */
  void checkDirectionChange();

  /**
   * @brief Computes the angle between two vectors.
   * @param v1 First vector.
   * @param v2 Second vector.
   * @return The angle between v1 and v2 in degrees.
   */
  double angleBetweenVectors(const Vector3& v1, const Vector3& v2);

 private:
  ///< Indicates if the drone is available for a new delivery.
  bool available = false;
  bool pickedUp = false;   ///< Indicates if the drone has picked up a package.
  Package* package = nullptr;  ///< Pointer to the current package, if any.
  IStrategy* toPackage = nullptr;  ///< Strategy to navigate to the package.
  ///< Strategy to navigate to the final destination.
  IStrategy* toFinalDestination = nullptr;
  std::vector<std::string> routeData = {};  ///< Data about the current route.
  Vector3 startPosition;  ///< Position at the time of package pickup.
  double totalDistance = 0;  ///< Total distance traveled by the drone.
  Vector3 previousDirection;  ///< Previous direction vector of the drone.
  int rightTurnCount = 0;  ///< Count of right turns made.
  int leftTurnCount = 0;  ///< Count of left turns made.
  double routeTimeElapsed = 0.0;  ///< Time elapsed since the start of the route
};

#endif
