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

        if (station.doitArreter()) {//si le train a fait tous ses tours
            loco.arreter();
            ++(*amountWaiting);//on informe les autres threads qu'on est en attente
            loco.afficherMessage(QString("%1").arg(*amountWaiting));
            sem->release();
            mutex->acquire();
            for (int i = 0; i < station.getNbTrains(); ++i) {
                sem->acquire();
            }

            if (*amountWaiting != 1) {
              //si ce train est le dernier à s'arrêter, on réveille tous les autres et on change le mode de priorité
                for (int i = 0; i < station.getNbTrains(); ++i) {
                    sem->release();
                }
                sharedSection->togglePriorityMode();
            }
            --(*amountWaiting);
            mutex->release();

            PcoThread::thisThread()->usleep(2'000'000);
            setPriority(getRandomPriority());
            loco.inverserSens();
            sensHoraire = !sensHoraire;
            loco.demarrer();
            attendre_contact(station.getNumeroContactGare()); // ignore hitting station because of inertia
        }

        int contactRequest = sensHoraire ? sharedSectionContacts.contactPremierRequest : sharedSectionContacts.contactSecondRequest;
        int contactEntree = sensHoraire ? sharedSectionContacts.contactPremierDebut : sharedSectionContacts.contactSecondDebut;
        int contactSortie = sensHoraire ? sharedSectionContacts.contactSecondFin : sharedSectionContacts.contactPremierFin;
        int directionPremierAiguillage = sharedSectionContacts.doitChangerVoie ? TOUT_DROIT : DEVIE;
        int directionSecondAiguillage = sharedSectionContacts.doitChangerVoie ? DEVIE : TOUT_DROIT;
        int premierAiguillage = sensHoraire ? sharedSectionAiguillages.premierAiguillageHoraire : sharedSectionAiguillages.premierAiguillageAntiHoraire;
        int secondAiguillage = sensHoraire ? sharedSectionAiguillages.secondAiguillageHoraire : sharedSectionAiguillages.secondAiguillageAntiHoraire;

        attendre_contact(contactRequest);
        sharedSection->request(loco, priority);
        attendre_contact(contactEntree);
        sharedSection->access(loco, priority);
        diriger_aiguillage(premierAiguillage, directionPremierAiguillage, 0);
        diriger_aiguillage(secondAiguillage, directionSecondAiguillage, 0);
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

void LocomotiveBehavior::setPriority(int priority) {
    this->priority = priority;
}

int LocomotiveBehavior::getRandomPriority() {
    return std::rand() % (MAX_PRIORITY + 1);
}
