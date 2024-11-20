//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "locomotivebehavior.h"
#include "ctrain_handler.h"
#include "pcosynchro/pcothread.h"

void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    while(true) {
        attendre_contact(station.getNumeroContactGare());
        station.incrementeCompteurTour();
        loco.afficherMessage(QString("J'ai atteint la gare %1").arg(station.getNumeroContactGare()));

        if (station.doitArreter()) {
            loco.arreter();
            ++(*amountWaiting);
            loco.afficherMessage(QString("%1").arg(*amountWaiting));
            sem->release();
            mutex->lock();
            for (int i = 0; i < station.getNbTrains(); ++i) {
                sem->acquire();
            }

            if (*amountWaiting != 1) {
                for (int i = 0; i < station.getNbTrains(); ++i) {
                    sem->release();
                }
            }
            --(*amountWaiting);
            mutex->unlock();

            PcoThread::thisThread()->usleep(2'000'000);
            loco.inverserSens();
            sensHoraire = !sensHoraire;
            loco.demarrer();
            attendre_contact(station.getNumeroContactGare()); // ignore hitting station because of inhertia
        }

        int contactEntree = sensHoraire ? sharedSectionAiguillages.contactPremierDebut : sharedSectionAiguillages.contactSecondDebut;
        int contactSortie = sensHoraire ? sharedSectionAiguillages.contactSecondFin : sharedSectionAiguillages.contactPremierFin;
        int directionPremierAiguillage = sharedSectionAiguillages.doitChangerVoie ? TOUT_DROIT : DEVIE;
        int directionSecondAiguillage = sharedSectionAiguillages.doitChangerVoie ? DEVIE : TOUT_DROIT;
        
        attendre_contact(contactEntree);
        sharedSection->access(loco);
        diriger_aiguillage(sharedSection->getPremierAiguillage(sensHoraire), directionPremierAiguillage, 0);
        diriger_aiguillage(sharedSection->getSecondAiguillage(sensHoraire), directionSecondAiguillage, 0);
        attendre_contact(contactSortie);
        sharedSection->leave(loco);
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
