#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cmath>
#include <vector>
#include <algorithm>

// Function to calculate IMSI based on distance and PRB
std::string calculateIMSI(double distance, int prb) {
    // Replace with your logic to calculate IMSI
    // For this example, we'll use a simple predefined value
    return "123456789";
}

// Function to calculate RNTI based on distance and PRB
int calculateRNTI(double distance, int prb) {
    // Replace with your logic to calculate RNTI
    // For this example, we'll use a simple predefined value
    return 12345;
}

int main() {
    double cellTowerFrequency;
    double deviceFrequency;
    double distance;
    int prb;

    // Define allowed PRB values
    std::vector<int> allowedPRBValues = {20, 50, 70};

    // Define allowed cell tower frequencies
    std::vector<double> allowedCellTowerFrequencies = {2140, 2660, 2810};

    // Predefined CellID
    int cellID = 22062619;

    // Print a message indicating that we are looking for available SDRs
    std::cout << "Looking for available SDRs..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Found compatible device blade_rf using SoapySDR" << std::endl;

    // Get cell tower frequency
    std::cout << "Enter cell tower frequency (in MHz): ";
    std::cin >> cellTowerFrequency;

    // Get device frequency
    std::cout << "Enter device frequency (in MHz): ";
    std::cin >> deviceFrequency;

    // Get distance from SDR
    std::cout << "Enter distance from SDR (in meters): ";
    std::cin >> distance;

    // Get PRB
    std::cout << "Enter PRB (must be <= 100): ";
    std::cin >> prb;

    // Check if the frequency is within range or exit with an error message
    if (std::find(allowedCellTowerFrequencies.begin(), allowedCellTowerFrequencies.end(), cellTowerFrequency) ==
        allowedCellTowerFrequencies.end()) {
        std::cout << "Searching for device..." << std::endl;
        for (int i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Searching..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds
        std::cerr << "No device found in the given frequency." << std::endl;
        return 1;
    }

    // Check if the frequency difference exceeds 50 Hz or exit with an error message
    if (std::abs(cellTowerFrequency - deviceFrequency) > 50) {
        std::cout << "Searching for device..." << std::endl;
        for (int i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Searching..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds
        std::cerr << "Error: Frequency difference exceeds SDR capacity." << std::endl;
        return 1;
    }

    // Check if the distance is valid or exit with an error message
    if (distance < 0.01 || distance > 1.0) {
        std::cout << "Searching for device..." << std::endl;
        for (int i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Searching..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds
        std::cerr << "No device found in the given distance." << std::endl;
        return 1;
    }

    // Check if PRB is valid or exit with an error message
    if (std::find(allowedPRBValues.begin(), allowedPRBValues.end(), prb) == allowedPRBValues.end()) {
        std::cout << "Searching for device..." << std::endl;
        for (int i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Searching..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds
        std::cerr << "No device found for the given PRB." << std::endl;
        return 1;
    }

    // Buffer period with status messages
    std::cout << "Searching for device..." << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Searching..." << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds
    std::cout << "Found device." << std::endl;

    // Decoding buffer period
    std::cout << "Decoding..." << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Define IMSI, RNTI, and Cell Tower ID values based on cell tower frequency
    std::string imsi;
    int rnti;

    if (cellTowerFrequency == 2140) {
        imsi = "922150000412873";
        rnti = 9213;
        cellID = 22062619;
    } else if (cellTowerFrequency == 2660) {
        imsi = "961110900542873";
        rnti = 2254;
        cellID = 27291548;
    } else if (cellTowerFrequency == 2810) {
        imsi = "900011140546573";
        rnti = 7781;
        cellID = 21498740;
    }

    // Output the results in a refined tabular form
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "           Device Information                " << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(20) << "CellID:" << cellID << std::endl;
    std::cout << std::left << std::setw(20) << "IMSI:" << imsi << std::endl;
    std::cout << std::left << std::setw(20) << "RNTI:" << rnti << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    return 0;
}
