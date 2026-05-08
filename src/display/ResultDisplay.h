//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_RESULTDISPLAY_H
#define FLIGHT_OPTIMIZER_RESULTDISPLAY_H

#include <vector>
#include <string>
#include "../models/FlightOffer.h"
#include "../models/UserQuery.h"

/**
 * Handles all terminal output for the flight optimizer. Formats and prints query results,
 * individual flight offers, and summary statistics.
 */
class ResultDisplay {
public:
    /**
     * Prints all flight offers returned for a query. Shows header with query summary,
     * then numbered list of offers. If offers is empty, prints a friendly 'no results' message.
     */
    static void printResults(const std::vector<FlightOffer>& offers, const UserQuery& query);

    /**
     * Prints a limited number of flight offers (pagination).
     * @param offers All available offers
     * @param query User query for header display
     * @param startIndex Starting index (0-based)
     * @param count Number of offers to display
     * @return Number of offers actually printed
     */
    static int printResultsPaginated(const std::vector<FlightOffer>& offers, const UserQuery& query, int startIndex, int count);

    /**
     * Prints a single FlightOffer with index number. Shows: total price, duration, stops,
     * score, seat class, and each leg's details.
     */
    static void printOffer(const FlightOffer& offer, int index);

    /**
     * Prints a single flight leg. Shows: leg number, flight number, airline,
     * origin→destination, departure/arrival times, and live status.
     */
    static void printLeg(const Flight& flight, int legNumber);

    /**
     * Prints a formatted summary of the user's query parameters (origin, destination,
     * date, seat class, budget, max stops, priority mode).
     */
    static void printQuerySummary(const UserQuery& query);

    /**
     * Prints a friendly message when no flight offers are found.
     */
    static void printNoResults();

    /**
     * Prints a horizontal separator line (e.g., 50 dashes) for visual formatting.
     */
    static void printSeparator();

    /**
     * Converts total minutes to a human-readable string like '2h 35m'.
     */
    static std::string formatDuration(int minutes);

    /**
     * Converts a unix timestamp to a human-readable local time string like '14:35 on 2025-04-30'.
     * Uses std::localtime and std::strftime.
     */
    static std::string formatTimestamp(long long unixTime);

    // seatClassToString is provided as a free inline function in UserQuery.h
};

#endif //FLIGHT_OPTIMIZER_RESULTDISPLAY_H