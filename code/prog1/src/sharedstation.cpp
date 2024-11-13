//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include <chrono>
#include <thread>

#include "sharedstation.h"

SharedStation::SharedStation(int numeroContactGare, int nbTrains, int nbTours) : numeroContactGare(numeroContactGare), nbTrains(nbTrains), n(nbTours), compteurTour(0)
{}

int SharedStation::getNumeroContactGare() {
    return numeroContactGare;
}

int SharedStation::getNbTrains() {
    return nbTrains;
}

void SharedStation::incrementeCompteurTour() {
    ++compteurTour;
}

bool SharedStation::doitArreter() {
    return (compteurTour % n) == 0;
}
