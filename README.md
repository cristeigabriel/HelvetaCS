# Helveta
 
Counter-Strike: Global Offensive Cheat base written in and with C++20 concepts.

# Features
- ImDrawList based self contained drawing manager which always knows any renderable's x/y/w/h/color,
- Concurrent multi-queue thread-safe rendering abilities (allows for both locked and not-locked queues, for, say, either ESP, or a menu overlay),
- Console UI framework with state callbacks that fail upon return false and access to the current, previewable, class context, bool/int/color identifiers with value preview and value setting, scrollable text zone, auto-suggest from identifiers and callbacks with selection scroll, and JSON config system embed to identifier creation (cached),
- Entity Cacher,
- Basic, animated/faded ESP (queued in FrameStageNotify, thus making the user able to use engine methods safely),
- Simple, example movement features such as Bunny Hop and No Duck Delay,
- Netvars that use Class Network name, names used for the structural make up of the cheat, and with compile-time hashes of the names stored in NETWORK_CLASS-es, with a compile-time property type parser for (more explicitly for Valve-oriented (enforced class member) Hungarian notation),
- Compile-time IDA style signature to Byte Array generation,
- Memory String finder with compile-time folding into an array for the pattern scanning process,
- GameOverlayRenderer hooking.

# Licensing
WTFPL, available in the LICENSE file