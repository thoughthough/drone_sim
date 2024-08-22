#include "Robot.h"
#include "Package.h"
#include "SimulationModel.h"

Robot::Robot(const JsonObject& obj) : IEntity(obj) {}

void Robot::setPosition(Vector3 pos_) { position = pos_; }
void Robot::teleportToRandomPos() {
    // random number between -1400 and 1500
    double new_x = -1400 + (rand() % static_cast<int>(2901));
    double new_z = -800 + (rand() % static_cast<int>(1601));
    position =  Vector3(new_x, 270, new_z);
}


void Robot::update(double dt) {
    if (package) {
        Vector3 old_pos = position;
        teleportToRandomPos();
        Vector3 new_pos = position;

        if (model) {
            JsonObject details;
            details["name"] = name;
            JsonArray start = {old_pos.x, old_pos.y, old_pos.z};
            JsonArray end = {new_pos.x, new_pos.y, new_pos.z};
            details["start"] = start;
            details["end"] = end;
            details["search"] = package->getStrategyName();

            requestedDelivery = true;
            // package->setRequiresDelivery(true);
            model->scheduleTrip(details);
            package = nullptr;
         }
    }
}
void Robot::receive(Package* p) {
    package = p;
}
