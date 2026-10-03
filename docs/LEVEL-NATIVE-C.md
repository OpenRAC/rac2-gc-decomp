# C natif des niveaux

Ce support ajoute une source, un catalogue et une revue propres à un overlay,
en complément des corps C du boot déjà placés dans cet overlay. Il conserve
les contrôles des corps complets et le contrôle final de tous les PT_LOAD.
Aucun ELF, objet ou octet du jeu ne doit être versionné.

Pour un identifiant issu de `config/overlays.json`, les chemins sont :

| Artefact | Chemin |
| --- | --- |
| Source écrite à la main | `candidates/levels/<id>.c` |
| Catalogue de frontières et identité | `config/level-native/<id>.json` |
| Revue des corps et de l'objet | `progress/level-candidates/<id>.json` |

`scripts/level_native.py` est l'API commune. Les symboles sont
`LVL_<ID_EN_MAJUSCULES>_FUN_<ADRESSE_8_HEX>`. Le catalogue contient schema 1,
kind `level-native-catalog`, target, level, program `levels/<id>`, reference_sha256
(pin de l'overlay), entry (entrée ELF), source canonique, flags, functions
(symbol/address/size/meaning), externals et éventuellement gp. Les tailles et
adresses sont des entiers multiples de quatre ; les corps ne se chevauchent pas.
Le catalogue de `24_ship_shack` donne un exemple réel, sans adresse figée dans l'API.

La qualification exige un ELF ET_EXEC avec la bonne entrée et le pin exact.
Chaque corps est entièrement contenu dans une section PROGBITS exécutable.
Elle compile dans un runtime privé, vérifie la fraîcheur, lie les sections
sélectionnées, vérifie la taille symbolique complète et compare chaque octet.
La revue enregistre les hashes de source, catalogue, vérificateur, objet,
ELF candidat, ELF référence et corps, ainsi que cpp/cc1/as/ld et les flags.
Les hashes des corps sont distincts des hashes des fichiers ELF.

Qualification depuis la racine du dépôt, avec les chemins privés adaptés :

```powershell
python -B scripts/check_level_candidates.py --level 24_ship_shack `
  --reference <overlay-prive.elf> --toolchain <chaine-C> `
  --runtime <runtime-hors-depot> --write-review
```

La reconstruction `build.py` conserve ses options existantes. `--c-level <id>`
sélectionne le C partagé et le C natif de ce niveau si le catalogue existe.
`--all-levels --c-all-levels --c-toolchain <chaine-C>` les sélectionne partout.
`--toolchain` désigne la chaîne de reconstruction ASM ; `--c-toolchain` désigne
la chaîne C. Ces deux instruments doivent rester distincts. Une revue boot
requalifiée peut être fournie explicitement par `--candidate-review <json>`
sans remplacer l'ancienne provenance.

L'intégration requalifie l'objet natif et exige le même hash que l'objet revu.
Les corps partagés gardent leur source `candidates/boot.c`, leur revue et leurs
contrôles. Chaque fragment du linker sélectionne son propre objet ; aucun
correctif de placement n'est ajouté. L'union refuse les chevauchements et les
externes contradictoires. La preuve schema 2, kind `level-c-integration`,
stocke les fonctions de l'union une seule fois, les deux qualifications,
les hashes des instruments de reconstruction et le gate chargé complet.

`decomp_report.py` accepte `--level-proof <integration.json>` (répétable),
`--candidate-review`, `--integration-proof` et `--progress-proof`. Il valide
chaque preuve sur son programme, sa source, sa revue, son objet et son gate,
puis dérive les compteurs. Les unités natives ont leur propre `sourcePath`.
Une preuve boot, un corps partiel ou un smoke check ne donnent aucun crédit.

La clé de dépendance d'un niveau contient ses trois fichiers natifs, son entrée
de placements partagés, la source/revue boot choisie et le vérificateur.
Changer le C d'un autre niveau ne l'invalide pas. Aucun index global des preuves
natives n'est nécessaire ; fournir les seules preuves encore valides à l'export.
Un changement de la source boot partagée invalide les niveaux qui la réutilisent.

Limites actuelles : les sources natives sont autonomes ; les includes et macros
de date/heure sont refusés jusqu'au pin des dépendances d'en-têtes. Le chemin
d'intégration mixte conserve l'exigence d'un catalogue partagé valide et non vide.
La démonstration initiale portait sur un seul corps natif de 24 octets dans un
overlay. Le [lot 18](EIGHTEENTH-C-LOT.md) valide maintenant le boot et les 27
overlays, avec deux corps natifs totalisant 104 octets dans `24_ship_shack`.
L'objectif de 10 % reste ouvert. Les revues JSON publiables ne contiennent que
les identités et mesures, pas les objets privés.
