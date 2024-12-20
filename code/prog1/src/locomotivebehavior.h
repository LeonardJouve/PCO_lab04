//    ___  _________    ___  ___  ___ ____ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  / / / //
//  / ___/ /__/ /_/ / / __// // / __/_  _/ //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include "locomotive.h"
#include "launchable.h"
#include "sharedstation.h"
#include "sharedsection.h"
#include "sharedsectioninterface.h"
#include "pcosynchro/pcomutex.h"
#include "pcosynchro/pcosemaphore.h"

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable {
  public:
    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(Locomotive &loco,
                       std::shared_ptr<SharedSectionInterface> sharedSection,
                       SharedStation station,
                       std::shared_ptr<PcoSemaphore> sem,
                       std::shared_ptr<PcoMutex> mutex,
                       std::shared_ptr<std::atomic<int>> amountWaiting,
                       struct SharedSectionAiguillages sharedSectionAiguillages/*, autres paramètres éventuels */) :
        loco(loco),
        sharedSection(sharedSection),
        station(station),
        sem(sem),
        mutex(mutex),
        amountWaiting(amountWaiting),
        sharedSectionAiguillages(sharedSectionAiguillages),
        sensHoraire(true) {
      // Eventuel code supplémentaire du constructeur
    }

  protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de la locomotive
     */
    void run() override;

    /*!
     * \brief printStartMessage Message affiché lors du démarrage du thread
     */
    void printStartMessage() override;

    /*!
     * \brief printCompletionMessage Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override;

    /**
     * @brief loco La locomotive dont on représente le comportement
     */
    Locomotive &loco;

    /**
     * @brief sharedSection Pointeur sur la section partagée
     */
    std::shared_ptr<SharedSectionInterface> sharedSection;

    /*
     * Vous êtes libres d'ajouter des méthodes ou attributs
     *
     * Par exemple la priorité ou le parcours
     */
    SharedStation station;

    //Ressources partagées entre les trains, sémaphores et mutex pour protéger les accès
    std::shared_ptr<PcoSemaphore> sem;
    std::shared_ptr<PcoMutex> mutex;
    std::shared_ptr<std::atomic<int>> amountWaiting;//nombre de trains en attente dans leurs gares respectives
    //structure contenant les points contacts des aiguillages de la section partagée
    struct SharedSectionAiguillages sharedSectionAiguillages;

    bool sensHoraire;//indique dans quel sens la locomotive est en train de rouler
};

#endif // LOCOMOTIVEBEHAVIOR_H
