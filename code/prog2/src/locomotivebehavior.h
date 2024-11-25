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
#include "pcosynchro/pcosemaphore.h"

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable
{
public:
    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(Locomotive& loco,
        std::shared_ptr<SharedSectionInterface> sharedSection,
        SharedStation station,
        std::shared_ptr<PcoSemaphore> sem,
        std::shared_ptr<PcoSemaphore> mutex,
        std::shared_ptr<std::atomic<int>> amountWaiting,
        struct SharedSectionContacts sharedSectionContacts,
        struct SharedSectionAiguillages sharedSectionAiguillages/*, autres paramètres éventuels */) :
        loco(loco),
        sharedSection(sharedSection),
        station(station),
        sem(sem),
        mutex(mutex),
        amountWaiting(amountWaiting),
        sharedSectionContacts(sharedSectionContacts),
        sensHoraire(true),
        sharedSectionAiguillages(sharedSectionAiguillages)
    {
        // Eventuel code supplémentaire du constructeur
        priority = getRandomPriority();
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
    Locomotive& loco;

    /**
     * @brief sharedSection Pointeur sur la section partagée
     */
    std::shared_ptr<SharedSectionInterface> sharedSection;

    /*
     * Vous êtes libres d'ajouter des méthodes ou attributs
     *
     * Par exemple la priorité ou le parcours
     */
    void setPriority(int priority);

    /**
     * Génère une nouvelle priorité aléatoire entre 1 et MAX_PRIORITY (10)
     * @return
     */
    static int getRandomPriority();

    SharedStation station;//la station où la locomotive doit s'arrêter

    //ressources partagées entres les threads
    std::shared_ptr<PcoSemaphore> sem;
    std::shared_ptr<PcoSemaphore> mutex;
    std::shared_ptr<std::atomic<int>> amountWaiting;

    struct SharedSectionContacts sharedSectionContacts;//les points de contacts d'access et request avant la section partagée
    bool sensHoraire;
    int priority;
    struct SharedSectionAiguillages sharedSectionAiguillages;//les aiguillages autour de la section partagée

    static const int MAX_PRIORITY = 10;
};

#endif // LOCOMOTIVEBEHAVIOR_H
