# CS-Caverun-Game
CS Caverun is a turn-based grid game written in C that demonstrates structured program design, state-driven logic, complex rule simulation, and algorithmic problem-solving. This project uses enums, structs, and modular functions to represent entities, board state, player actions, and environmental mechanics.

**Game Rules & Environment** 
A turn-based grid simulation implemented on a fixed 10×10 board. Each cell stores a single entity value (e.g. DIRT, WALL, BOULDER, GEM, LAVA, PLAYER), and the entire game state is updated synchronously on each input command.

**Game State Model**
- The board is represented as a 2D array of tile structs.
- Tile behavior is determined by an enum-based entity system.
- Player position is derived from board state rather than tracked separately, ensuring a single source of truth.

**Input & Movement**
- Player input is parsed one command at a time and translated into state transitions.
- Movement commands (W/A/S/D) perform boundary checks and collision validation before updating the board.
- Invalid moves are ignored without mutating state.
- Dash input allows two sequential moves in a single turn, with stamina constraints enforced across turns.
  
**Physics & Environment Updates**
- Boulder movement is gravity-driven and processed after player actions.
- Gravity direction is configurable (up/down/left/right), requiring directional iteration order to maintain deterministic behavior.
- Boulder–player collisions trigger life loss and conditional respawn logic.
  
**Lava Simulation**
- Lava behavior is implemented using neighbor-count rules similar to a cellular automaton.
- Two modes are supported: continuous lava evolution and lava seeding.
- Lava updates occur after movement and physics resolution to preserve update order correctness.
  
**Visibility System**
- An illumination mode computes Euclidean distance from the player and masks tiles beyond a given radius.
- Visibility affects rendering only and does not modify underlying game state.
  
**Win/Loss Conditions**
- The exit unlocks when all collectible gems are removed from the board.
- The game ends when the player reaches the unlocked exit, loses all lives, or cannot respawn due to blocked conditions.
