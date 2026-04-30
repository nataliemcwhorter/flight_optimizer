//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_DATALOADER_H
#define FLIGHT_OPTIMIZER_DATALOADER_H

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "../models/Airport.h"
#include "../models/Flight.h"
#include "../models/Airline.h"
#include "../structures/FlightGraph.h"

/**
 * Loads static CSV datasets into the flight optimizer.
 *
 * Data sources:
 *   - OurAirports CSV format (airports with geographic coordinates)
 *   - OpenFlights routes CSV format (flight routes)
 *   - OpenFlights airlines CSV format (airline metadata)
 *
 * All methods are static — no instance of DataLoader needs to be created.
 * Call DataLoader::loadAirports(...), etc. directly.
 */
class DataLoader {
public:
    /**
     * Parse OurAirports CSV and add each Airport to the graph.
     * Expects columns: id, name, city, country, iataCode, icaoCode, latitude, longitude, ...
     */
    static void loadAirports(const std::string& csvPath, FlightGraph& graph);

    /**
     * Parse OpenFlights routes CSV and add each Flight to the graph.
     * Expects columns: airlineCode, airlineId, sourceIata, sourceId, destIata, destId, codeshare, stops, equipment, ...
     */
    static void loadRoutes(const std::string& csvPath, FlightGraph& graph);

    /**
     * Parse OpenFlights airlines CSV and populate airlines map keyed by iataCode.
     * Expects columns: id, name, alias, iataCode, icaoCode, callsign, country, active, ...
     */
    static void loadAirlines(const std::string& csvPath, std::unordered_map<std::string, Airline>& airlines);

private:
    /**
     * Parse a single CSV row from the OurAirports dataset.
     * Returns an Airport struct populated with parsed fields.
     */
    static Airport parseAirportRow(const std::string& row);

    /**
     * Parse a single CSV row from the OpenFlights routes dataset.
     * Returns a Flight struct with iataCode fields set; full Airport objects resolved later.
     */
    static Flight parseRouteRow(const std::string& row);

    /**
     * Parse a single CSV row from the OpenFlights airlines dataset.
     * Returns an Airline struct populated with parsed fields.
     */
    static Airline parseAirlineRow(const std::string& row);
};

#endif //FLIGHT_OPTIMIZER_DATALOADER_H