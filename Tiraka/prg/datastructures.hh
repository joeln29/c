// Datastructures.hh
//
// Student name: Joel Niskanen

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>


// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    //
    // Old assignment 1 operations
    //

    // Estimate of performance: O(1)
    // Short rationale for estimate: size() depends on the size of the datastructure
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Clearing vector or unordered_map is a linear operation, worst case linear
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Returning a variable is a linear operation
    std::vector<StationID> all_stations();

    // Estimate of performance: O(1)
    // Short rationale for estimate: On average, inserting into a unordered_map is constant operation
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(1)
    // Short rationale for estimate: on average, find() is constant time operation
    Name get_station_name(StationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: on average, find() is constant time operation
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: std::sort() is, on average, linearithmic operation
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: std::sort() is, on average, linearithmic operation,
    // std::is_sorted() is linear operation
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: worst case the whole unordered_map has to be looped through
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: on average, find() is constant time operation, worst case linear
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(n)
    // Short rationale for estimate: on average, find() is constant time operation (worst case linear)
    // and adding into a vector is constant time operation
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: on average, find() is constant time operation, but worst case the whole datastructure
    // has to be looped through which is a linear operation
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: on average, find() is constant time operation, worst case linear,
    // and adding into a vector is constant time operation
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: On average, inserting into a unordered_map is constant operation,
    // worst case linear
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Returning a variable is a constant time operation
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(1)
    // Short rationale for estimate: on average, find() is constant time operation
    Name get_region_name(RegionID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: on average, find() is constant time operation
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: on average, find() is constant time operation and adding into
    // a vector is constant time operation
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(1)
    // Short rationale for estimate: on average, find() is constant time operation and adding into
    // a vector is constant time operation
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: on average, find() is constant time operation (worst case linear)
    // and adding into a vector is constant time operation
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_station(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: worst case regions struct must be looped through
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

    //
    // New assignment 2 operations
    //

    // Estimate of performance: O(n)
    // Short rationale for estimate: adding into vector is linear operation
    bool add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes);

    // Estimate of performance: O(n)
    // Short rationale for estimate: on average, find() is constant time operation
    std::vector<StationID> next_stations_from(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: on average, find() is constant time operation
    std::vector<StationID> train_stations_from(StationID stationid, TrainID trainid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: clear() is linear time operation
    void clear_trains();

    // Estimate of performance: O(n)
    // Short rationale for estimate: BFS is on average linear time operation
    std::vector<std::pair<StationID, Distance>> route_any(StationID fromid, StationID toid);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Distance>> route_least_stations(StationID fromid, StationID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> route_with_cycle(StationID fromid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Distance>> route_shortest_distance(StationID fromid, StationID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<StationID, Time>> route_earliest_arrival(StationID fromid, StationID toid, Time starttime);

private:
    // Struct for information about railway stations
    struct Region;
    struct Stop{
        Name name;
        StationID SID;
        Region* RID = nullptr;
        Coord coords;
        Time time = NO_TIME;
        TrainID TID = NO_TRAIN;
        std::vector<std::pair<Time,TrainID>> departures = {};
        std::unordered_map<TrainID, Stop*> neighbours = {};
        Stop* previous = nullptr;
    };
    // Different vectors to store all SIDs
    std::vector<StationID> station_IDs;
    std::unordered_map<StationID, Stop> stations;
    std::vector<Stop*> alphabetical_order;
    std::vector<Stop*> coordinates;

    std::unordered_map<TrainID, std::vector<std::pair<StationID, Time>>> trains;

    // Struct for information about region
    struct Region{
        Name name;
        RegionID RID;
        std::vector<Coord> coords;
        Region* parentreg = nullptr;
        std::vector<RegionID> subregions = {};
        std::vector<Stop*> stations = {};
    };
    // Vector to store all RIDs
    std::vector<RegionID> region_IDs;
    std::unordered_map<RegionID, Region> regions;

};

#endif // DATASTRUCTURES_HH
