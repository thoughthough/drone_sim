#ifndef DATA_COLLECTOR_H
#define DATA_COLLECTOR_H

#include <vector>
#include <string>
#include "util/json.h"

/**
 * @class DataCollector
 * @brief Singleton class responsible for collecting and storing data from the simulation.
 *
 * This class manages a centralized collection of data strings related to the state of entities within 
 * the simulation. It ensures that there is only one instance of the collector throughout the application 
 * lifecycle, providing a global point of access to the collected data.
 */
class DataCollector {
 private:
  ///< Static instance for the singleton pattern.
  static DataCollector* instance;

  std::vector<std::string> collectedData;  ///< Vector to store data strings.
  DataCollector() {}  ///< Private constructor to enforce singleton pattern.

 public:
  /**
   * @brief Destructor that might handle tasks like writing data to a file.
   */
  ~DataCollector();

  /**
   * @brief Retrieves the singleton instance of the DataCollector.
   * @return Pointer to the singleton instance of DataCollector.
   */
  static DataCollector* GetInstance();

  /**
   * @brief Collects a vector of data strings.
   * @param data Vector of strings to be collected.
   */
  void collectData(const std::vector<std::string>& data);

  /**
   * @brief Collects state data of a specific entity.
   * @param id The identifier of the entity.
   * @param state JSON object representing the state of the entity.
   */
  void collectEntityState(int id, const JsonObject& state);

  /**
   * @brief Gets all collected data.
   * @return A constant reference to the vector storing all collected data strings.
   */
  const std::vector<std::string>& getCollectedData() const;

  /**
   * @brief Clears all the collected data.
   */
  void clearData();

  /**
   * @brief Performs any necessary cleanup operations.
   */
  void cleanup();

  ///< Copy constructor is deleted.
  DataCollector(const DataCollector&) = delete;
  ///< Copy assignment operator is deleted.
  DataCollector& operator=(const DataCollector&) = delete;
};

#endif  // DATA_COLLECTOR_H
