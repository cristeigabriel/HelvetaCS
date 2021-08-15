# Helveta
 
Counter-Strike: Global Offensive Cheat base written in and with C++20 concepts.

# Features
- ImDrawList based self contained drawing manager which always knows any renderable's x/y/w/h/color,
- Concurrent multi-queue thread-safe rendering abilities (allows for both locked and not-locked queues, for, say, either ESP, or a menu overlay),
- Console UI framework with state callbacks that fail upon return false and access to the current, previewable, class context, bool/int/color identifiers with value preview and value setting, scrollable text zone, auto-suggest from identifiers and callbacks with selection scroll, and JSON config system embed to identifier creation (cached),
- Entity Cacher,
- Basic, animated/faded ESP (queued in FrameStageNotify, thus making the user able to use engine methods safely), currently offering the ability to display the bounding box outlines of players, their names above it, their weapons below it, and footsteps around with their name - again - and localization information (Footstep map position, actively last stored, labeled position),
- Unlock ConVars Console callback,
- Simple, example movement features such as Bunny Hop and No Duck Delay,
- Netvars that use Class Network name, names used for the structural make up of the cheat, and with compile-time hashes of the names stored in NETWORK_CLASS-es, with a compile-time property type parser for (more explicitly for Valve-oriented (enforced class member) Hungarian notation),
- Compile-time IDA style signature to Byte Array generation,
- Memory String finder with compile-time folding into an array for the pattern scanning process,
- GameOverlayRenderer hooking.

# Building Process
I, the author, personally use the following:
- Visual Studio Code
- - Plugins
- - - CMake
- - - CMake Tools
- - - C++
- Visual Studio 2022 Build Tools (x86, v143) CMakeTools kit with MSBuild (tools obtainable from Visual Studio Installer)

With the aforementioned, what's left to do is to prompt the CMake plugin to set up its environment in the IDE, and offer me utilities or specific IDE configuration edits according to my current ones in comparassion to the project's, then pressing 'Build'.

# Licensing
WTFPL, available in the LICENSE file
