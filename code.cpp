#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

class Vehicle {
public:
    int id;
    int waitingTime;

    Vehicle(int i, int wait) : id(i), waitingTime(wait) {}
};

class Lane {
public:
    string direction;
    vector<Vehicle> vehicles;

    Lane(string dir) : direction(dir) {}

    void addVehicle(const Vehicle& v) {
        vehicles.push_back(v);
    }

    void moveVehicles() {
        if (!vehicles.empty()) {
            vehicles.erase(vehicles.begin());  // Remove the first vehicle
        }
    }

    void updateWaitingTimes() {
        for (auto& vehicle : vehicles) {
            vehicle.waitingTime++;
        }
    }

    void displayLane() {
        cout << direction << " Lane: ";
        for (const auto& vehicle : vehicles) {
            cout << "[ID: " << vehicle.id 
                 << ", Wait: " << vehicle.waitingTime << "s] ";
        }
        cout << endl;
    }
};

class TrafficSystem {
private:
    vector<Lane> lanes;
    int greenLaneIndex;
    int timeStep;
    int timer;
    int vehicleCounter;

public:
    TrafficSystem() : greenLaneIndex(0), timeStep(0), timer(5), vehicleCounter(1) {
        lanes.emplace_back("North");
        lanes.emplace_back("South");
        lanes.emplace_back("East");
        lanes.emplace_back("West");
    }

    void generateVehicle() {
        int randomLane = rand() % lanes.size();
        lanes[randomLane].addVehicle(Vehicle(vehicleCounter++, 0));
    }

    void simulate() {
        while (timeStep < 30) {  // Run for 30 time steps
            system(CLEAR);

            cout << "Traffic Simulation\n";
            cout << "------------------\n";

            // Display signal state for each lane
            for (size_t i = 0; i < lanes.size(); ++i) {
                if (i == greenLaneIndex) {
                    cout << "Signal (" << lanes[i].direction << "): GREEN (" << timer << "s remaining)\n";
                } else {
                    cout << "Signal (" << lanes[i].direction << "): RED\n";
                }
            }
            cout << endl;

            // Display all lanes
            for (auto& lane : lanes) {
                lane.updateWaitingTimes();  // Increment waiting time for vehicles
                lane.displayLane();
            }

            // Generate a vehicle every 5 time steps
            if (timeStep % 5 == 0) {
                generateVehicle();
            }

            // Move vehicles on the green lane
            lanes[greenLaneIndex].moveVehicles();

            // Manage signal timer
            timer--;
            if (timer == 0) {
                // Change green light to the next lane
                greenLaneIndex = (greenLaneIndex + 1) % lanes.size();
                timer = 5;  // Reset timer for the next green signal
            }

            // Increment time step
            timeStep++;

            // Pause simulation
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    TrafficSystem trafficSystem;
    trafficSystem.simulate();

    return 0;
}

