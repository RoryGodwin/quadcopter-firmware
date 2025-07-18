#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

// Structure to hold GPS location and time data
struct GPSData {
    float latitude = 0.0f;
    float longitude = 0.0f;
    float altitude = 0.0f;
    int satellites = 0;
    bool fix = false;     // true if location fix is valid
};

// Abstract class for any GPS module
class GPSManager {
public:
    virtual void init() = 0;                      // Initialize the GPS module
    virtual bool read(GPSData& data) = 0;         // Read current GPS data
    virtual ~GPSManager() = default;              // Virtual destructor
};

#endif // GPS_MANAGER_H
