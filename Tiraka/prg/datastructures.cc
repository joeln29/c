// Datastructures.cc
//
// Student name: Joel Niskanen
// Student email: joel.niskanen@tuni.fi


#include "datastructures.hh"
#include <random>
#include <cmath>
#include <queue>
#include <unordered_set>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator


template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

}

/**
 * @brief Datastructures::station_count() how many stations
 * @return int, station count
 */
unsigned int Datastructures::station_count()
{
    return station_IDs.size();
}

/**
 * @brief Datastructures::clear_all clears all datastructures
 */
void Datastructures::clear_all()
{
    station_IDs.clear();
    stations.clear();
    alphabetical_order.clear();
    coordinates.clear();

    region_IDs.clear();
    regions.clear();

    trains.clear();
}

/**
 * @brief Datastructures::all_stations, get every stations IDs
 * @return all station IDs
 */
std::vector<StationID> Datastructures::all_stations()
{
    return station_IDs;
}

/**
 * @brief Datastructures::add_station, adds new station
 * @param id, id for the new station
 * @param name, name for the new station
 * @param xy, x and y coordinates for the new station
 * @return true if adding was successful, false if not
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy){
    auto found = stations.find(id);
    if(found != stations.end()){
       return false;
    } else {
        station_IDs.push_back(id);
        stations[id] = Stop{name, id, nullptr, xy, NO_TIME, NO_TRAIN};
        Stop* pointer = &stations[id];
        alphabetical_order.push_back(pointer);
        coordinates.push_back(pointer);
        return true;
    }
}

/**
 * @brief Datastructures::get_station_name, get the name of station
 * @param id, search key
 * @return station name if the station was found, else NO_NAME
 */
Name Datastructures::get_station_name(StationID id){
    auto found_id = stations.find(id);
    if(found_id != stations.end()){
       return found_id->second.name;
    } else {
        return NO_NAME;
    }
}

/**
 * @brief Datastructures::get_station_coordinates, get the station coordinates
 * @param id, search key
 * @return return coordinates if the station was found, else NO_COORD;
 */
Coord Datastructures::get_station_coordinates(StationID id){

    auto found_coord = stations.find(id);
    if(found_coord != stations.end()){
       return found_coord->second.coords;
    } else {
        return NO_COORD;
    }
}

/**
 * @brief Datastructures::stations_alphabetically, sort stations alphabetically by name
 * @return return names in sorted vector
 */
std::vector<StationID> Datastructures::stations_alphabetically(){
    std::vector<StationID> vector;
    auto lambda = [](Stop* const first, Stop* const second)
                    {return first->name < second->name;};

    std::sort(alphabetical_order.begin(), alphabetical_order.end(), lambda);
    for(auto i : alphabetical_order){
        vector.push_back(i->SID);
    }
    return vector;
}

/**
 * @brief Datastructures::stations_distance_increasing, sort stations distance increasing
 * @return return names in sorted vector
 */
std::vector<StationID> Datastructures::stations_distance_increasing(){
    auto lambda = [](Stop* const first, Stop* const second){
        auto x = std::sqrt(std::pow(first->coords.x, 2) + std::pow(first->coords.y, 2));
        auto y = std::sqrt(std::pow(second->coords.x, 2) + std::pow(second->coords.y, 2));

        if(x == y){
            return first->coords.y < second->coords.x;
        } else {
            return x < y;
        }
    };
    std::vector<StationID> vector;
    std::sort(coordinates.begin(), coordinates.end(), lambda);
    for(auto i : coordinates){
        vector.push_back(i->SID);
    }
    return vector;
}

/**
 * @brief Datastructures::find_station_with_coord, find station by coordinates
 * @param xy, x and y coordinates for the search
 * @return stationid if found, else NO_STATION
 */
StationID Datastructures::find_station_with_coord(Coord xy){
    for(auto &i : stations){
        if(i.second.coords == xy){
           return i.second.SID;
        }
    }
    return NO_STATION;
}

/**
 * @brief Datastructures::change_station_coord, change station coordinates
 * @param id for finding the right station
 * @param newcoord, new coordinates
 * @return true if changing was successful, false if not
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord){
    auto found_id = stations.find(id);
    if(found_id != stations.end()){
        found_id->second.coords = newcoord;
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Datastructures::add_departure, adds new departure
 * @param stationid, for finding the right station
 * @param trainid, id for indentifying trains
 * @param time when the train departures
 * @return true if adding was successful, false if not
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time){
    auto found_id = stations.find(stationid);
    if(found_id != stations.end()){
        found_id->second.departures.push_back(std::make_pair(time, trainid));
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Datastructures::remove_departure, remove departure from a station
 * @param stationid for finding the right station
 * @param trainid for finding the correct train
 * @param time for finding the correct train (both need
 * @return true if removing was successful, false if not
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time){
    auto found_id = stations.find(stationid);
    if(found_id != stations.end()){
        for(auto i = found_id->second.departures.begin(); i != found_id->second.departures.end(); i++){
            if(i->first == time && i->second == trainid){
                found_id->second.departures.erase(i);
                break;
            }
        }
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Datastructures::station_departures_after, list departures by time
 * @param stationid for finding the right station
 * @param time, list trains leaving after given time
 * @return vector of leaving trains, if no trains leaving return NO_TIME, NO_TRAIN in a vector
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time){
    auto found_id = stations.find(stationid);
    if(found_id != stations.end()){
        // Trains leaving at given time are saved in this temp. vector
        std::vector<std::pair<Time, TrainID>> vector;
        for(auto i = found_id->second.departures.begin(); i != found_id->second.departures.end(); i++){
            if(i->first >= time){
                vector.push_back(std::make_pair(i->first, i->second));
            }
        }
        return vector;
    } else {
        return {std::make_pair(NO_TIME, NO_TRAIN)};
    }
}

/**
 * @brief Datastructures::add_region, adds new region
 * @param id, id for the new region
 * @param name, name for the new region
 * @param coords for the new region coordinates
 * @return true if adding was successful, false if not
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords){
    auto found = regions.find(id);
    if(found != regions.end()){
        return false;
    } else {
        region_IDs.push_back(id);
        regions[id] = Region{name, id, coords};
        return true;
    }
    return true;
}

/**
 * @brief Datastructures::all_regions, get every region IDs
 * @return region IDs in a vector
 */
std::vector<RegionID> Datastructures::all_regions(){
    return region_IDs;
}

/**
 * @brief Datastructures::get_region_name, get the name of a region
 * @param id, id for finding the wanted region
 * @return region name if the region was found, else return NO_NAME
 */
Name Datastructures::get_region_name(RegionID id){
    auto found_id = regions.find(id);
    if(found_id != regions.end()){
       return found_id->second.name;
    } else {
        return NO_NAME;
    }
}

/**
 * @brief Datastructures::get_region_coords, get the coordinates of a region
 * @param id, id for finding the wanted region
 * @return vector, coordinates if the region was found, else return NO_COORD in a vector
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id){
    auto found_coord = regions.find(id);
    if(found_coord != regions.end()){
        return found_coord->second.coords;
    } else {
        return {NO_COORD};
    }
}

/**
 * @brief Datastructures::add_subregion_to_region, adds subregion into a region
 * @param id, id for finding the region which to be added
 * @param parentid, finding the region in which subregion is added
 * @return true if adding was successful, false if not
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid){
    auto found_region = regions.find(id);
    auto found_parent = regions.find(parentid);
    if(found_region == regions.end() || found_parent == regions.end()
            || found_region->second.parentreg != nullptr){
        return false;
    } else {
        found_region->second.parentreg = &found_parent->second;
        found_parent->second.subregions.push_back(id);
        return true;
    }
}

/**
 * @brief Datastructures::add_station_to_region, adds station to region
 * @param id, id for the station to be added
 * @param parentid, the region in which station is to be added
 * @return true if adding was successful, false if not
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid){
    auto found_station = stations.find(id);
    auto found_region = regions.find(parentid);
    if((found_station == stations.end() || found_region == regions.end())
            || found_station->second.RID != nullptr){
        return false;
    } else {
        found_station->second.RID = &found_region->second;
        found_region->second.stations.push_back(&stations.at(id));
        return true;
    }
}

/**
 * @brief Datastructures::station_in_regions, list all regions in which the station is located
 * @param id, id for finding the wanted station
 * @return vector, where all regions are listed, if no regions are found return NO_REGION
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id){
    std::vector<RegionID> vector;
    auto found_id = stations.find(id);
    // Check if station ID exists
    if(found_id != stations.end()){
        // Check if station doesn't belong to any regions
        if(found_id->second.RID == nullptr){
            return {};
        } else {
            auto temp = found_id->second.RID;
            while(temp != nullptr){
                vector.push_back(temp->RID);
                temp = temp->parentreg;
            }
            return vector;
        }
    } else {
        return {NO_REGION};
    }
}

std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID /*id*/){
    // Replace the line below with your implementation
    // Also uncomment parameters (/* param */ -> param)
    throw NotImplemented("all_subregions_of_region()");
}

std::vector<StationID> Datastructures::stations_closest_to(Coord /*xy*/){
    // Replace the line below with your implementation
    // Also uncomment parameters (/* param */ -> param)
    throw NotImplemented("stations_closest_to()");
}

bool Datastructures::remove_station(StationID /*id*/){
    // Replace the line below with your implementation
    // Also uncomment parameters (/* param */ -> param)
    throw NotImplemented("remove_station()");
}

/**
 * @brief Datastructures::common_parent_of_regions, find the common parent of two stations
 * @param id1, for finding the right station and its parent regions
 * @param id2, for finding the right station and its parent regions
 * @return if found common parent ID, else NO_REGION
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2){
    auto found_id1 = regions.find(id1);
    auto found_id2 = regions.find(id2);
    if(found_id1 == regions.end() && found_id2 == regions.end()){
       return NO_REGION;
    }
    if(found_id1->second.parentreg == found_id2->second.parentreg){
        return found_id1->second.RID;
    } else {
        std::vector<RegionID> temp;
        Region* parent_reg = found_id1->second.parentreg;
        while(parent_reg != nullptr){
            temp.push_back(parent_reg->RID);
            parent_reg = parent_reg->parentreg;
        }
        parent_reg = found_id2->second.parentreg;
        while(parent_reg != nullptr){
            if(std::find(temp.begin(), temp.end(), parent_reg->RID) != temp.end()){
                return parent_reg->RID;
            }
        }
    }
    return NO_REGION;
}

/**
 * @brief Datastructures::add_train add train into the datastructure
 * @param trainid individual train ID
 * @param stationtimes vector pair of station ID and time the train travels
 * @return return true if adding was successful
 */
bool Datastructures::add_train(TrainID trainid, std::vector<std::pair<StationID, Time>> stationtimes){
    auto found_train = trains.find(trainid);
    if(found_train != trains.end()){
        return false;
    } else {
        std::vector<Stop*> temp_stations = {};
        // käy läpi kaikki asemat
        for(auto &i : stationtimes){
            auto found_SID = stations.find(i.first);
            // jos jokin asema ei löydy return false, muuten lisää temp vektoriin pointteri minne juna kulkee
            if(found_SID == stations.end()){
                return false;
            } else {
                temp_stations.push_back(&found_SID->second);
            }
        }
        // lisätään uusi juna mappiin
        trains[trainid] = stationtimes;
        //
        for(unsigned int i = 0;i < temp_stations.size() - 1; i++){
            temp_stations[i]->departures.push_back({stationtimes[i].second, trainid});
            temp_stations[i]->neighbours[trainid] = temp_stations[i+1];
        }
        temp_stations.back()->departures.push_back({stationtimes.back().second, trainid});
        return true;
    }
}
/**
 * @brief Datastructures::next_stations_from return all following stations
 * @param id key to find wanted station where to search from
 * @return return vector of stations
 */
std::vector<StationID> Datastructures::next_stations_from(StationID id){
    auto found_station = stations.find(id);
    if(found_station == stations.end()){
        return {NO_STATION};
    } else {
        std::vector<StationID> temp;
        for(auto &i : found_station->second.neighbours){
            temp.push_back(i.second->SID);
        }
        return temp;
    }
}

/**
 * @brief Datastructures::train_stations_from list of stations train will pass on its' way
 * @param stationid the starting station
 * @param trainid the train which path we want to follow
 * @return
 */
std::vector<StationID> Datastructures::train_stations_from(StationID stationid, TrainID trainid){
    auto found_SID = stations.find(stationid);
    auto found_TID = trains.find(trainid);
    if(found_SID == stations.end() || found_TID == trains.end()){
        return {NO_STATION};
    } else {
        auto iter = found_SID->second.neighbours.find(trainid);
        if(iter == found_SID->second.neighbours.end()){
            return {NO_STATION};
        }
        std::vector<StationID> temp;
        auto current_station = iter->second;
        while(iter->second){
            temp.push_back(iter->second->SID);
            iter = iter->second->neighbours.find(trainid);
            if(iter == current_station->neighbours.end()){
                break;
            }
            current_station = iter->second;
        }
        return temp;
    }
}

/**
 * @brief Datastructures::clear_trains clear datastructures
 */
void Datastructures::clear_trains(){
    trains.clear();
    for(auto &i : stations){
        i.second.departures.clear();
        i.second.neighbours.clear();
    }
}

/**
 * @brief distance calculate distance between two points
 * @param a the first point
 * @param b the second point
 * @return return the calculated distance
 */
int distance(Coord a, Coord b){
    return trunc(sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2)));
}

/**
 * @brief Datastructures::route_any return some path between two stations
 * @param fromid station where to start the search
 * @param toid station where to stop the search
 * @return return all stations in order and the overall distance
 */
std::vector<std::pair<StationID, Distance>> Datastructures::route_any(StationID fromid, StationID toid){
    auto found_station = stations.find(fromid);
    auto found_station2 = stations.find(toid);
    if(found_station == stations.end() || found_station2 == stations.end()){
        return {{NO_STATION,NO_DISTANCE}};
    }
    // Set pointers to nullptr so we know when we are at the first station in reverse order
    found_station->second.previous = nullptr;
    found_station2->second.previous = nullptr;

    std::vector<std::pair<StationID, Distance>> temp;
    // BFS
    std::queue<Stop*> que;
    std::unordered_set<StationID> visited_nodes;
    visited_nodes.insert(found_station->second.SID);
    que.push(&found_station->second);
    while(!que.empty()){
        Stop* current_node = que.front();
        que.pop();
        for(auto &[trainID, stop] : current_node->neighbours){
            auto not_visited = visited_nodes.insert(stop->SID).second;
            if(not_visited == true){
                que.push(stop);
                stop->previous = current_node;
                if(stop == &found_station2->second){
                    while(!que.empty()){
                        que.pop();
                    }
                    break;
                }
            }
        }
    }
    if(found_station2->second.previous == nullptr){
       return temp;
    }
    std::vector<std::pair<StationID, Distance>> reverse_route;
    int sum = 0;
    auto prev_station = &found_station2->second;
    while(prev_station->previous != nullptr){
        reverse_route.push_back({prev_station->SID,
                        distance(prev_station->coords, prev_station->previous->coords)});
        prev_station = prev_station->previous;
    }
    reverse_route.push_back({prev_station->SID, 0});
    temp.reserve(reverse_route.size());
    for(auto i = reverse_route.rbegin(); i != reverse_route.rend(); i++){
        sum += i->second;
        temp.push_back({i->first, sum});
    }
    return temp;
}


std::vector<std::pair<StationID, Distance>> Datastructures::route_least_stations(StationID fromid, StationID toid){
    return route_any(fromid, toid);
}

std::vector<StationID> Datastructures::route_with_cycle(StationID /*fromid*/){
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_with_cycle()");
}

std::vector<std::pair<StationID, Distance>> Datastructures::route_shortest_distance(StationID /*fromid*/, StationID /*toid*/){
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_shortest_distance()");
}

std::vector<std::pair<StationID, Time>> Datastructures::route_earliest_arrival(StationID /*fromid*/, StationID /*toid*/, Time /*starttime*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("route_earliest_arrival()");
}
