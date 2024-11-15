//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "locomotivebehavior.h"
#include "ctrain_handler.h"
#include "pcothread.h"

void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    /* A vous de jouer ! */

    // Vous pouvez appeler les méthodes de la section partagée comme ceci :
    //sharedSection->access(loco);
    //sharedSection->leave(loco);

    while(true) {
        // On attend qu'une locomotive arrive sur le contact 1.
        // Pertinent de faire ça dans les deux threads? Pas sûr...
        attendre_contact(station.getNumeroContactGare());
        station.incrementeCompteurTour();

        if (station.doitArreter()) {
            loco.arreter();
            sem.release();
            mutex.lock();
            for (int i = 0; i < station.getNbTrains(); ++i) {
                sem.require();
            }

            for (int i = 0; i < station.getNbTrains(); ++i) {
                sem.release();
            }
            mutex.unlock();

            PcoThread::thisThread->usleep(2_000_000);
            sem.release();
            loco.inverserSens();
            loco.demarrer();
        }

        loco.afficherMessage("J'ai atteint la gare " + station.getNumeroContactGare());
    }
}

void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}
