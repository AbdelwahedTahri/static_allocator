# Présentation 

Le code comprend deux fonctions `static_malloc()` et `static_free()` qui servent à faire des allocations statiques de mémoire comparable aux fonctions `malloc()` et `free()` de la bibliothèque standard.

Les deux fonctions fourniront de la mémoire à partir d'un espace préalablement allouée statiquement.


# Static_malloc

```C

void *static_malloc(size_t taille);

```

La fonction reçoit une taille en multiplet et retourne un pointeur vers un bloc de mémoire d'**au moins la taille demandée** ou un **pointeur nul** s’il n’y a plus de mémoire disponible.

Afin de réaliser ces allocations, la fonction ira piocher dans un bloc de mémoire statique.
Dans notre cas un tableau de char de classe de stockage statique d’une taille prédéterminée

```C

static _Alignas(max_align_t) char mem[1048576UL]; // 1024 * 1024 multiplet

```

Étant donné que la fonction ne connaît pas les contraintes d’alignements que doit respecter l’objet qui sera stocké dans le bloc qu’elle fournit, elle doit retourner un bloc respectant les contraintes les plus strictes  `_Alignof(max_align_t)`

Le tableau alloué statiquement doit lui aussi être aligné suivant **l’alignement le plus sévère**


## La libération de la mémoire

Si nous voulons libérer des blocs puis les réutiliser, il nous faut conserver d’une manière ou d’une autre leur taille

Pour ce faire, il nous est possible d’ajouter un **en-tête** lors de l’allocation d’un bloc
autrement dit un objet qui précédera le bloc alloué et contiendra la taille du bloc.

Ainsi, lors de l’allocation, il nous suffit de transmettre à l’utilisateur le bloc alloué sans son en-tête

```text
      En-tête                                En-tête
<------------------>                   <------------------>
+--------+---------+-------------+     +--------+---------+-------------+
| Taille | Suivant | Bloc alloué |     | Taille | Suivant | Bloc alloué |
+--------+---------+-------------+     +--------+---------+-------------+
               +                       ^
               |                       |
               +-----------------------+

Taille  : La taille du bloc alloué courant
Suivant : Un pointeur vers un bloc precedemment libéré 

```

Encore une fois la taille d’en-tête doit être «arrondie» de sorte que les données qui seront stockées dans le bloc respectent **l’alignement le plus strict** .


## Fragmentation et défragmentation

Un problème subsiste : la **fragmentation** de la mémoire allouée, autrement dit sa division en une multitude de petits blocs.

Imaginez que nous ayons alloué toute la mémoire sous forme de blocs de 16, 32 et 64 multiplets, si même tous ces blocs sont **libres**, notre allocateur retournera un **pointeur nul** dans le cas d’une demande de par exemple 80 multiplets.

Une solution consiste à défragmenter la liste des blocs libres, c’est-à-dire fusionner plusieurs blocs pour en reconstruire d’autre avec une taille plus importante.

La fonction doit donc regarder si un bloc est d’une taille suffisante ou si plusieurs blocs, une fois fusionnés, seront de taille suffisante.


# Static_free

```C

void static_free(void *Block);

```

La fonction `static_free()` comme `free()` libère un objet précédemment allouée, elle reçoit en paramètre un pointeur sur l'objet à libérer .

## La libération de la mémoire et  la défragmentation

Comme il est déjà mentionné,  si nous voulons libérer des blocs puis les réutiliser, il nous faut conserver leur taille dans l'en-tête qui représente une liste chaînée dont chaque élément contient la taille du bloc .

`static_free()` donc se charge d'ajouter les blocs libérés à la liste .

Nous avons aussi parler du problème de la **fragmentation de la mémoire** et de la solution qui consiste à faire de la **défragmentation** qui fusionne les blocs .

Toutefois, une fusion de blocs n’est possible que si ceux-ci sont adjacents, c’est-à-dire s’ils
se suivent en mémoire. Néanmoins, il ne nous est pas possible de vérifier cela facilement si notre liste de blocs libres n’est pas un minimum triée.

il faut donc trier notre liste par adresses croissantes (ou décroissantes, le résultat sera le même) de sorte que si un bloc n’est pas adjacent au suivant, la recherche peut être immédiatement arrêtée pour ce bloc ainsi que tous ceux qui lui étaient adjacents. 

# Le fichier main

Le fichier main présente un exemple de programme qui utilise les deux fonctions, et n'a pas d'autre intérêts.





