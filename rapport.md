Kunzli Christophe, Jouve Leonard

# Introduction

Dans ce labo nous devons implémenter une logique de gestion concurentielle de trains passant par des sections partagés,
avec une notion de priorité d'accès. Les trains ne doivent pas se croiser dans les sections partagées.

# Choix de conception

La logique principale du comportement des trains se trouvent dans la classe locomotivebehavior.

Chaque train est lié à une station qui est en dehors de la section partagée, cette station est unique pour chaque train.
Une fois que tous les trains ont atteints leur station, ils attendent pendant 2 secondes. Cette attente est réalisée
par l'appel à la méthode usleep() sur leur threads.

Lorsque un train arrive à sa gare et s'arrête, il release un semaphore (partagé avec les autres trains par un
shared_ptr), rentre en section critique, et acquire ce même semaphore n fois (ou n est le nombre de trains qu'il doit
attendre avant de pouvoir partir). Une fois que tous les trains sont arrivés, il peut avancer dans le code pour
effectuer un release en cascade sur le semaphore afin de libérer les autres trains. Il sort ensuite de la section
critique.

Le bouton arrêt d'urgence met simplement la vitese de tous les trains à 0.

Pour la section partagé, nous avons ajouté un vecteur contenant les priorités des trains qui souhaitent rentrer dans la
section partagée, dont les écriture et lecture sont protegé par un mutex. Ces trains doivent s'arrêter avant la section
si elle est déjà occupée ou si ils ne peuvent pas obtenir la priorité.

A la fin de la méthode access, un mutex est locké puis release dans la méthode release par le train qui a eu accès à la
section.

# Validation

Scénarios de Tests :

1. Test de Priorité Initiale :

Attribuer des priorités différentes aux deux locomotives.
Vérifier que la locomotive avec la priorité la plus élevée accède à la section partagée en premier.

2. Test de Priorité Égale :

Donner la même priorité aux deux locomotives.
S'assurer que la première locomotive à atteindre la section partagée est prioritaire.

3. Test de Modification Dynamique des Priorités :

Après l'attente en gare, vérifier que les priorités changent aléatoirement.
Confirmer que la locomotive avec la nouvelle priorité la plus élevée a la priorité d'accès.

4. Test de Gestion de Contacts :

Simuler le passage de chaque locomotive par les contacts de requête et d'acceptation.
Vérifier le comportement correct selon que la section est libre ou occupée.

5. Test d'Inversion des Priorités :

Après chaque attente en gare, s'assurer que les priorités HIGH et LOW sont inversées systématiquement.

6. Test de Robustesse :

Introduire des scénarios où les deux locomotives tentent simultanément d'accéder à la section critique.
Observer le respect strict des règles de priorité.

# Conclusion

Le résultat semble conforme à la conception demandée.

Il reste un problème à résoudre, la boucle d'attente active lorsque un train doit attendre sur un autre train avant de
pouvoir entrer dans la section partagée cause parfois des problèmes de performance. Il faudrait trouver une solution.
