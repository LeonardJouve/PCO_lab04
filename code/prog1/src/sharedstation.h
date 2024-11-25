#ifndef SHARED_STATION_H
#define SHARED_STATION_H

#include <pcosynchro/pcosemaphore.h>

class SharedStation
{
public:
    SharedStation(int numeroContactGare, int nbTrains, int nbTours);

    /* Implémentez toute la logique que vous avez besoin pour que les locomotives
     * s'attendent correctement à la station */

    /**
     * getter pour le numero de contact de la gare
     * @return
     */
    int getNumeroContactGare();

    /**
     * getter pour le nombre de trains qui passent par le segment partagé
     * @return
     */
    int getNbTrains();

    /**
     * incremente le compteur de tour quand un train passe à sa gare
     */
    void incrementeCompteurTour();

    /**
     * determine si un train doit s'arreter à la gare
     * @return
     */
    bool doitArreter();

private:
    /* TODO */
    int numeroContactGare; /* numero du point de contact de la gare */
    int nbTrains; /* nombre de train à attendre */
    int n; /* nombre de tour avant de faire demi-tour */
    int compteurTour; /* nombre de tour effectués */
};

#endif // SHARED_STATION_H
