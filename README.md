# Pengo

A simple C++ tile-based arcade game inspired by classic Pengo. Supports single-player, local co-op, data-driven levels, and a basic scoring & high-score system.

---

## What Pengo Is

- **Tile-based** game world loaded from JSON  
- **Single-player** & **local co-op** mode  
- **Sliding/breaking walls**, moving/killing enemies 
- **Score tracking** with high-score screen  
- **Audio** & **input** via a central Service Locator

---

## Key Design Choices

1. **Component-Based GameObjects**  
   - Every entity (player, wall, egg, enemy) is a `GameObject` with components (Sprite, Collision, AI, etc.).  
   - Mix-and-match behaviors without deep inheritance trees.

2. **Data-Driven Levels**  
   - Levels defined in JSON (`width`, `height`, list of tiles).  

3. **Loose Coupling with Patterns**  
   - **Observer**: Enemies fire events to a `ScoreObserver`, which updates the HUD.  
   - **Command**: Keyboard / gamepad input creates `MoveCommand` and `SoundCommand` objects that can be rebound at runtime.  
   - **Service Locator**: Single access point for systems (audio, input, collision) instead of scattered globals.

4. **State Machine for Scenes**  
   - States: `MainMenuState`, `SinglePlayerState`, `CoOpState`, `HighScoreState`.  
   - `GameStateManager` handles transitions via `OnEnter` / `Update` / `OnExit`.

---

## Why?

- **Flexibility**: Components let me add new behaviors without rewriting core classes.  
- **Modularity**: Patterns separate concerns—collision code doesn’t touch scoring, input code doesn’t touch rendering.  
- **Fast Iteration**: JSON levels speed up testing and balance tweaks.
