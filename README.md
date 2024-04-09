# ShooterMechanics
The purpose of this repository is to store all the related files and code that I develop for game mechanics of an FPS in Unreal Engine 5. This will allow for easy access and reproducibility of specific mechanics whenever I need them for future projects.

Video updates of the project: https://www.youtube.com/watch?v=sYh329YvWUA&list=PLATKA12HJuo72EW57XFp7ftF2AAM3RUrq&pp=gAQBiAQB

## Key highlights so far:
- Created a Player Character with full body mesh and FPS mesh, collision component, and basic movement (walk, sprint, jump).
- Created an animation blueprint for locomotion, that changes based on whether the character is armed or unarmed.
- Implemented a base weapon class with some basic capabilities to start off. Separate child classes will be created for different weapon types later.
- Created a weapon attachment function that attaches the weapon to the character's hands when equipped.
- Added physics to the weapons when unequipped or dropped (shown in video).
- Added a weapon fire functionality and line tracing (as seen in the video) to debug where any projectiles shot from the weapon will travel.
- Created a new weapon collision channel
- Implemented weapon pickup system
- Added an adaptive crosshair that changes based on whether the character is looking at an interactable object or not
- Implemented a very clunky aim down sights system