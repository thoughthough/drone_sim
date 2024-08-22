#include "DataCollector.h"
#include<fstream>
#include <cstdlib>
using std::vector;

DataCollector *DataCollector::instance = nullptr;

DataCollector* DataCollector::GetInstance() {
    if (instance == nullptr) {
        instance = new DataCollector();
    }
    return instance;
}

DataCollector::~DataCollector() {
    cleanup();
}

void DataCollector::collectEntityState(int id, const JsonObject& state) {
    std::cout << "Collecting state for entity "
    << id << ": " << state.toString() << std::endl;
}

void DataCollector::collectData(const  std::vector<std::string>& data) {
  // std::cout << "Data collected: " << data << std::endl;
  std::string record = "";
  for (const auto& d : data) {
    record += d + ",";
  }
  collectedData.push_back(record);
  std::cout.flush();
}

void DataCollector::clearData() {
    collectedData.clear();
}

const std::vector<std::string>& DataCollector::getCollectedData() const {
    return collectedData;
}

void DataCollector::cleanup() {
    std::ofstream file("logs/collectedData.csv");  // Open a CSV file

    std::cout << "Final output of collected data:\n";

    // Write header to the CSV file
    if (file.is_open()) {
        std::string header =
            "Strategy,Start,End,Distance Taveled,"
            "Elapsed Time,Right Turns,Left Turns,";
        file << header << "\n";
        for (const auto& data : collectedData) {
            // std::cout << data << std::endl;
            file << data << "\n";
        }

        file.flush();  // Ensure all data is written to the file

        // Call the first Python script
        int result = std::system("python3 logs/generate_graph.py");
        if (result != 0) {
            std::cerr << "Failed to execute first"
                      << " Python script with error code "
                      << result << std::endl;
        }

        // Call the second Python script
        result = std::system("python3 logs/generate_graph_thirteen_plus.py");
        if (result != 0) {
            std::cerr << "Failed to execute second"
                      << " Python script with error code "
                      << result << std::endl;
        }

        // Call the third Python script
        result = std::system("python3 logs/generate_graph_fourteen_plus.py");
        if (result != 0) {
            std::cerr << "Failed to execute third"
                      << " Python script with error code "
                      << result << std::endl;
        }

        // Call the forth Python script
        result = std::system("python3 logs/generate_graph_sixteen_plus.py");
        if (result != 0) {
            std::cerr << "Failed to execute forth"
                      << " Python script with error code "
                      << result << std::endl;
        }

        // Call the fith Python script
        result = std::system("python3 logs/generate_graph_seventeen_plus.py");
        if (result != 0) {
            std::cerr << "Failed to execute fith"
                      << " Python script with error code "
                      << result << std::endl;
        }

        // Call the sixth Python script
        result = std::system("python3 logs/generate_graph_eighteen_plus.py");
        if (result != 0) {
            std::cerr << "Failed to execute sixth"
                      << " Python script with error code "
                      << result << std::endl;
        }

        file.close();  // Close the file after writing
    } else {
        std::cerr << "Failed to open the file for writing." << std::endl;
    }
    clearData();
}
