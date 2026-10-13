# BTT Studio 1.0 — Practice Mod 1.0 (Super Mario Odyssey 1.0.0)

> **Version :** `v1.0.0`  
> **Jeu compatible :** Super Mario Odyssey 1.0.0 (Title ID: `0100000000010000`)  
> **Plateformes :** Nintendo Switch (Atmosphere CFW / Simple Mod Manager) & Émulateur Yuzu  
> **Binaire :** `exefs/subsdk4` (~598 Ko) + `exefs/main.npdm`

---

## 🌟 Points Forts de la Mise à Jour v1.0.0

Cette mise à jour majeure unifie le **moteur natif de Super Mario Odyssey 1.0.0** (préservant tous les glitchs iconiques de speedrun tels que le *First Person Glitch*) avec la superbe **interface verticale translucide et moderne de la version 1.3** (police officielle Rodin haute définition).

### 🌈 1. Input Display Déplaçable & Couleur Dynamique "Multicolor" (RGB Rainbow)
- **Déplacement libre à la souris (Drag & Drop) :** Lorsque le menu est ouvert, un contour interactif discret entoure la manette. Cliquez et glissez simplement pour placer l'Input Display exactement où vous le souhaitez.
- **Contrôle précis et presets :**
  - **Preset Pos :** *Center*, *Bottom Left*, *Bottom Right*, *Top Left*, *Top Right* pour caler instantanément le contrôleur dans les 4 coins de l'écran.
  - **Position X & Position Y :** Réglage fin par pas de 25 pixels.
  - **Reset Position :** Rétablit la manette au centre.
- **Couleur "Multicolor" (RGB Rainbow dynamique) :**
  - Génération procédurale d'un dégradé arc-en-ciel animé en continu via conversion dynamique HSV.
  - Applicable indépendamment ou simultanément pour : **Button Color**, **Pressed Color**, **Stick Color**, **Ring Color** et **BG Color** (avec opacité adaptée pour le fond).
  - Support complet des manettes Player 1 et Player 2.

### 🎥 2. Caméra Instantanée & Vitesse Zéro sur Load Position
- **Zéro délai de transition :** Annulation immédiate de l'interpolation de la caméra (`al::requestCancelCameraInterpole`) verrouillée sur une boucle de 10 frames dès le chargement de position (`D-Pad Droite`). La vue se cale instantanément sur Mario sans aucun glissement visuel.
- **Annulation de la vitesse résiduelle (`al::setVelocityZero`) :** Mario ne subit aucune secousse ni inertie de son déplacement précédent.
- **Support des captures / transformations (`hack`) :** La sauvegarde et la téléportation gèrent parfaitement les ennemis capturés (Grenouille, Goomba, etc.).

### 🔄 3. Reload at Pos / Instant Reload
- **Rechargement de scène à la position actuelle :** Recharge la scène complète tout en replaçant immédiatement Mario à ses coordonnées et orientation actuelles, avec vie restaurée et sans cinématique d'introduction.
- **Option "Reload at Pos" (Yes/No) :** Dans les catégories **Options** et **Misc** (synchronisé avec ImGui).
- **Bouton d'action directe "Restart at Pos" :** Dans la catégorie **Misc** pour recharger à la demande en 1 clic.
- **Machine d'état robuste (`mReloadState`) :** Garantit zéro bug de désynchronisation entre le déchargement de l'ancien monde et la création de Mario dans le nouveau.

### 🪙 4. Les 18 Pixel Luigi 3D SMB1 Authentiques (100% Fidèles)
- Les 18 emplacements secrets officiels de Super Mario Odyssey reproduits avec une précision 1:1 d'après la vidéo officielle `1011.mp4` :
  1. Anneau de poussière d'impact au sol lors de la charge (`ZL`/`ZR`).
  2. Jaillissement en arc de cercle culminant à 280 unités avec pirouette verticale.
  3. Plaquage au sol à plat ventre horizontal.
  4. Rebond dynamique et redressement debout fier face caméra.
  5. Flèche blanche indicatrice ⬇ animée en flottement.
  6. Deux grosses pièces d'or 3D en rotation avec étincelles rétro scintillantes.
  7. Réaction de cabriole si Mario ou Cappy le touche.
  8. Réinitialisation automatique au **Restart Scene** (+200 pièces ré-obtenues à chaque reset pour s'entraîner aux routes de speedrun).

### 🔒 5. Isolation Complète des Entrées en Menu
- Quand le menu vertical est ouvert, les entrées manette (sauts, déplacements de Mario, caméra, interactions Cappy) sont isolées du moteur de jeu. Mario reste parfaitement immobile.
- Détection stricte des raccourcis hotkeys pour éviter toute téléportation involontaire en naviguant dans le menu.

### 🎩 6. Sélecteur d'Outfits (42 Costumes & 42 Chapeaux)
- Sélecteurs complets **"Outfit Body"** et **"Outfit Cap"** dans la catégorie **Misc**.
- Bouton **"Apply Outfit"** appliquant instantanément la tenue et rechargeant la scène.

### ⚡ 7. Speedrun Refreshes Intégrés
- **Cutscene Refresh :** Réactive toutes les cinématiques de monde, de scénario, la première capture de Bonneton et les dialogues de tous les PNJ majeurs (Capitaine Toad, Hint Toad, Jammin' Toad, Traveling Peach).
- **Warp Text Refresh :** Réactive l'explication complète de Cappy lors du premier contact avec un checkpoint.
- **Stage Warper :** Téléportation instantanée dans tous les mondes et zones annexes du jeu.

### ⌨️ 8. Raccourcis Manette Personnalisables
- Choix parmi 20 combinaisons pour **Save Position**, **Load Position** et **Restart Scene** directement depuis le menu **Keybinds**.

---

## 🎮 Raccourcis par Défaut

| Raccourci | Action |
|---|---|
| **L + D-Pad Gauche** | Ouvrir / Fermer le menu vertical BTT Studio |
| **D-Pad Gauche** | Sauvegarder la position (Save State) |
| **D-Pad Droite** | Charger la position (Load State - Caméra instantanée) |
| **D-Pad Haut** | Recharger la scène (Restart Scene / Reload at Pos) |
| **D-Pad Bas** | Scène précédente (Prev Stage) |
| **Maintenir R3 (Stick Droit)** | Roue d'actions rapides (Quick Wheel) |

---

## 📦 Instructions d'Installation

### 1. Sur Émulateur Yuzu (PC)
1. Téléchargez l'archive ou récupérez les fichiers `exefs/subsdk4` et `exefs/main.npdm`.
2. Placez-les dans votre dossier de mod Yuzu :
   ```
   %APPDATA%\yuzu\load\0100000000010000\Practice mod 1.0\exefs\subsdk4
   %APPDATA%\yuzu\load\0100000000010000\Practice mod 1.0\exefs\main.npdm
   ```
3. Dans Yuzu, faites clic droit sur **Super Mario Odyssey** &rarr; **Propriétés** &rarr; cochez **Practice mod 1.0** (désactivez la 1.3 si présente).
4. Lancez le jeu (version SMO 1.0.0).

### 2. Sur Nintendo Switch (Simple Mod Manager)
1. Copiez le dossier `Practice mod 1.0` dans `SD:/mods/` :
   ```
   SD:/mods/Practice mod 1.0/contents/0100000000010000/exefs/subsdk4
   SD:/mods/Practice mod 1.0/contents/0100000000010000/exefs/main.npdm
   ```
2. Ouvrez l'application **Simple Mod Manager** sur votre Switch et activez le mod.

### 3. Sur Nintendo Switch (Atmosphere Direct)
1. Copiez les fichiers dans :
   ```
   SD:/atmosphere/contents/0100000000010000/exefs/subsdk4
   SD:/atmosphere/contents/0100000000010000/exefs/main.npdm
   ```
2. Démarrez votre console avec Atmosphere.

---

## 👥 Crédits & Remerciements
- **MrKatzenGaming** — Développeur de BTT-Studio 1.0 original
- **fruityloops1 / tetraxile** — Auteurs de smo-practice et smo-practice-2 (Practice Mod 1.3)
- **CraftyBoss** — SMO-Exlaunch-Base
- **MonsterDruide1** — OdysseyHeaders
- **ocornut** — Dear ImGui
