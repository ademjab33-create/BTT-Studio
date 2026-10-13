# BTT Studio — Practice Mod 1.0

[![Release](https://img.shields.io/badge/Release-v1.0.0-blue.svg)](GITHUB_RELEASE_NOTES.md)
[![Game](https://img.shields.io/badge/Game-Super_Mario_Odyssey_1.0.0-red.svg)](#)
[![TitleID](https://img.shields.io/badge/Title_ID-0100000000010000-green.svg)](#)
[![Platform](https://img.shields.io/badge/Platform-Nintendo_Switch_%7C_Yuzu-purple.svg)](#)
[![Docs](https://img.shields.io/badge/Showcase-GitHub_Pages-cyan.svg)](docs/index.html)

> **BTT Studio Practice Mod 1.0** est la fusion ultime entre le moteur natif de **Super Mario Odyssey 1.0.0** (préservant tous les glitchs et mécaniques de speedrun) et la superbe interface graphique verticale translucide du **Practice Mod 1.3** (police Rodin).

---

## 🌐 Démo & Showcase Interactif

Un site web vitrine interactif est disponible dans le dossier [`docs/`](docs/index.html) (compatible **GitHub Pages**) :
- **Simulateur d'Input Display en direct** : déplacez la manette à la souris (drag & drop) et testez les presets.
- **Effet Multicolor (RGB Rainbow)** animé en temps réel.
- **Comparatif interactif des versions** (SMO Vanilla vs Practice Mod 1.3 vs BTT Studio 1.0).
- **Guide pas-à-pas** et copie des chemins d'accès en 1 clic.

---

## 🚀 Nouveautés de la Version 1.0

### 🌈 1. Input Display Déplaçable & Couleur Multicolor
- **Drag & Drop libre :** Déplacez directement l'Input Display à l'écran à la souris lorsque le menu est ouvert.
- **Presets & Réglages :** Presets 4 coins (*Center*, *Bottom Left*, *Bottom Right*, *Top Left*, *Top Right*) et réglage fin X/Y par pas de 25 pixels.
- **Couleur Multicolor (RGB Rainbow) :** Onde arc-en-ciel animée en continu pour tous les boutons, sticks, anneaux et fond.

### 🎥 2. Caméra Instantanée sur Load Position
- Annulation multi-frame de l'interpolation de la caméra (`al::requestCancelCameraInterpole`) et annulation de la vitesse résiduelle (`al::setVelocityZero`).
- Verrouillage instantané sur Mario à la première frame sans aucun glissement visuel.
- Support complet des transformations et captures (`hack`).

### 🔄 3. Reload at Pos (Rechargement à la position)
- Recharge entièrement la scène tout en replaçant Mario exactement à ses coordonnées actuelles avec vie restaurée.
- Option toggle "Reload at Pos" (Yes/No) et bouton direct "Restart at Pos".

### 🪙 4. Les 18 Pixel Luigi 3D SMB1 Authentiques
- Reproduction 1:1 exacte de l'animation officielle (vidéo `1011.mp4`) :
  - Onde de choc d'impact au sol
  - Saut en cloche à 280 unités
  - Plaquage horizontal au sol
  - Redressement fier avec flèche ⬇ indicatrice
  - 2 grosses pièces d'or 3D en rotation avec étincelles rétro
  - Réinitialisation automatique au Restart Scene (+200 pièces pour vos routes).

### 🔒 5. Blocage des Entrées du Jeu en Menu
- Mario reste immobile pendant la navigation dans le menu vertical (aucune entrée manette envoyée au jeu).

### 🎩 6. Sélecteur d'Outfits (42 Costumes & Chapeaux)
- Choix complet de 42 costumes et 42 chapeaux avec bouton **"Apply Outfit"** instantané.

### ⚡ 7. Speedrun Refreshes Intégrés
- **Cutscene Refresh :** Réactive toutes les cinématiques et dialogues PNJ (Capitaine Toad, Hint Toad, Jammin' Toad, Traveling Peach).
- **Warp Text Refresh :** Réactive les explications Cappy des checkpoints.

---

## 🎮 Contrôles & Raccourcis

| Raccourci | Fonction |
|---|---|
| **L + D-Pad Gauche** | Ouvrir / Fermer le menu vertical BTT Studio |
| **D-Pad Gauche** | Sauvegarder la position (`Save State`) |
| **D-Pad Droite** | Charger la position (`Load State` - Caméra instantanée) |
| **D-Pad Haut** | Recharger la scène (`Restart Scene` / `Reload at Pos`) |
| **D-Pad Bas** | Scène précédente (`Prev Stage`) |
| **Maintenir R3 (Stick Droit)** | Roue rapide d'actions (`Quick Wheel`) |

*Les touches de Save Pos, Load Pos et Restart Scene sont entièrement configurables dans le menu **Keybinds**.*

---

## 📥 Installation

### Sur Émulateur Yuzu
Placez les fichiers dans :
```
%APPDATA%\yuzu\load\0100000000010000\Practice mod 1.0\exefs\subsdk4
%APPDATA%\yuzu\load\0100000000010000\Practice mod 1.0\exefs\main.npdm
```
Dans Yuzu, faites clic droit sur **Super Mario Odyssey** &rarr; **Propriétés** &rarr; cochez **Practice mod 1.0**.

### Sur Nintendo Switch (Simple Mod Manager)
Copiez dans votre carte SD :
```
SD:/mods/Practice mod 1.0/contents/0100000000010000/exefs/subsdk4
SD:/mods/Practice mod 1.0/contents/0100000000010000/exefs/main.npdm
```
Activez le mod dans **Simple Mod Manager**.

### Sur Nintendo Switch (Atmosphere)
Copiez directement dans :
```
SD:/atmosphere/contents/0100000000010000/exefs/subsdk4
SD:/atmosphere/contents/0100000000010000/exefs/main.npdm
```

---

## 👥 Crédits

- [BTT-Studio](https://github.com/MrKatzenGaming/BTT-Studio) par **MrKatzenGaming**
- [smo-practice-2](https://github.com/fruityloops1/smo-practice-2) par **fruityloops1**
- [smo-practice](https://github.com/tetraxile/smo-practice) & [Calypso](https://github.com/tetraxile/Calypso) par **tetraxile**
- [smo-lunakit](https://github.com/Amethyst-szs/smo-lunakit) par **Amethyst-szs**
- [SMO-Exlaunch-Base](https://github.com/CraftyBoss/SMO-Exlaunch-Base) par **CraftyBoss**
- [OdysseyHeaders](https://github.com/MonsterDruide1/OdysseyHeaders) par **MonsterDruide1**
- [Dear ImGui](https://github.com/ocornut/imgui) par **ocornut**
