//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#ifndef SHAREDSECTION_H
#define SHAREDSECTION_H

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "sharedsectioninterface.h"

#ifndef SHARED_SECTION_STRUCT_2
#define SHARED_SECTION_STRUCT_2

// Structure pour les points de contact
struct SharedSectionContacts {
    int contactPremierRequest;
    int contactPremierDebut;
    int contactPremierFin;
    int contactSecondRequest;
    int contactSecondDebut;
    int contactSecondFin;
    bool doitChangerVoie;
};

struct SharedSectionAiguillages {
    int premierAiguillageHoraire;
    int secondAiguillageHoraire;
    int premierAiguillageAntiHoraire;
    int secondAiguillageAntiHoraire;
};

#endif

/**
 * @brief La classe SharedSection implémente l'interface SharedSectionInterface qui
 * propose les méthodes liées à la section partagée.
 */
class SharedSection final : public SharedSectionInterface
{
public:

    /**
     * @brief SharedSection Constructeur de la classe qui représente la section partagée.
     * Initialisez vos éventuels attributs ici, sémaphores etc.
     */
    SharedSection() : 
        mutex(1),
        isUsed(false),
        priorityMode(SharedSectionInterface::PriorityMode::HIGH_PRIORITY),
        requestedPriorities(),
        mutexPriority(1)
    {
        // TODO
    }

    /**
     * @brief request Méthode a appeler pour indiquer que la locomotive désire accéder à la
     * section partagée (deux contacts avant la section partagée).
     * @param loco La locomotive qui désire accéder
     * @param locoId L'identidiant de la locomotive qui fait l'appel
     * @param entryPoint Le point d'entree de la locomotive qui fait l'appel
     */
    void request(Locomotive& loco, int priority) override {
        // TODO
        mutexPriority.acquire();
        requestedPriorities.push_back(priority);
        mutexPriority.release();

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString("The engine no. %1 requested the shared section with priority %2.").arg(loco.numero()).arg(priority)));
    }

    /**
     * @brief access Méthode à appeler pour accéder à la section partagée, doit arrêter la
     * locomotive et mettre son thread en attente si la section est occupée par une autre locomotive.
     * Si la locomotive et son thread ont été mis en attente,
     * le thread doit être reveillé lorsque la section partagée est à nouveau libre et
     * la locomotive redémarée. (méthode à appeler un contact avant la section partagée).
     * @param loco La locomotive qui essaie accéder à la section partagée
     */
    void access(Locomotive &loco, int priority) override {
        int vitesse = loco.vitesse();
        bool stopRequired = false;
        while (true) {
            mutexPriority.acquire();
            
            if (isUsed && !stopRequired) {
                //si la section est déjà  utilisée le train doit s'arrêter
                stopRequired = true;
                arreter_loco(loco.numero());
            }

            std::vector<int>::iterator currentPriority = getCurrentPriority();
            if (priority == *currentPriority) {
                //si le train a la priorité il peut quitter la file d'attente et accéder à la section
                requestedPriorities.erase(currentPriority);
                isUsed = true;
                mutexPriority.release();
                break;
            }

            //si le train n'a pas la priorité il doit attendre
            if (!stopRequired) {
                stopRequired = true;
                arreter_loco(loco.numero());
            }

            mutexPriority.release();
        }

        mutex.acquire();
        loco.fixerVitesse(vitesse);
        // TODO

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString("The engine no. %1 accesses the shared section.").arg(loco.numero())));
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section
     * partagée. (reveille les threads des locomotives potentiellement en attente).
     * @param loco La locomotive qui quitte la section partagée
     */
    void leave(Locomotive& loco) override {
        isUsed = false;
        mutex.release();
        // TODO

        // Exemple de message dans la console globale
        afficher_message(qPrintable(QString("The engine no. %1 leaves the shared section.").arg(loco.numero())));
    }

    /**
     * swicth le mode de priorité après chaque tour
     */
    void togglePriorityMode() {
        priorityMode = priorityMode == SharedSectionInterface::PriorityMode::HIGH_PRIORITY ?
            SharedSectionInterface::PriorityMode::LOW_PRIORITY :
            SharedSectionInterface::PriorityMode::HIGH_PRIORITY;
    }
private:

    /**
     * determine quel train a la priorité en fonction du mode de priorité
     * @return
     */
    std::vector<int>::iterator getCurrentPriority() {
        return priorityMode == SharedSectionInterface::PriorityMode::HIGH_PRIORITY ?
            std::max_element(requestedPriorities.begin(), requestedPriorities.end()) :
            std::min_element(requestedPriorities.begin(), requestedPriorities.end());
    }

    // Méthodes privées ...
    // Attribut privés ...
    PcoSemaphore mutex;
    bool isUsed;// indique si un train est dans la section partagée
    enum SharedSectionInterface::PriorityMode priorityMode;
    std::vector<int> requestedPriorities;// liste des priorités des trains qui ont demandé l'accès
    PcoSemaphore mutexPriority;// pour protéger la liste des priorités
};


#endif // SHAREDSECTION_H
