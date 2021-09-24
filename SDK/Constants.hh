#pragma once

#define MAX_STUDIO_BONES 256

#define FCVAR_DEVELOPMENTONLY (1 << 1)  // Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.

#define FL_ONGROUND    (1 << 0)  // At rest / on the ground
#define FL_DUCKING     (1 << 1)  // Player flag -- Player is fully crouched
#define FL_ANIMDUCKING (1 << 2)  // Player flag -- Player is in the process of crouching or uncrouching but could be in transition \
                                // examples:                                   Fully ducked:  FL_DUCKING &  FL_ANIMDUCKING        \
                                //           Previously fully ducked, unducking in progress:  FL_DUCKING & !FL_ANIMDUCKING        \
                                //                                           Fully unducked: !FL_DUCKING & !FL_ANIMDUCKING        \
                                //           Previously fully unducked, ducking in progress: !FL_DUCKING &  FL_ANIMDUCKING
#define FL_WATERJUMP   (1 << 3)  // player jumping out of water
#define FL_ONTRAIN     (1 << 4)  // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN      (1 << 5)  // Indicates the entity is standing in rain
#define FL_FROZEN      (1 << 6)  // Player is frozen for 3rd person camera
#define FL_ATCONTROLS  (1 << 7)  // Player can't move, but keeps key inputs for controlling another entity
#define FL_CLIENT      (1 << 8)  // Is a player
#define FL_FAKECLIENT  (1 << 9)  // Fake client, simulated server side; don't send network messages to them
// NON-PLAYER SPECIFIC (i.e., not used by GameMovement or the client .dll ) -- Can still be applied to players, though
#define FL_INWATER (1 << 10)  // In water

// NOTE if you move things up, make sure to change this value
#define PLAYER_FLAG_BITS 11

#define FL_FLY           (1 << 11)  // Changes the SV_Movestep() behavior to not need to be on ground
#define FL_SWIM          (1 << 12)  // Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
#define FL_CONVEYOR      (1 << 13)
#define FL_NPC           (1 << 14)
#define FL_GODMODE       (1 << 15)
#define FL_NOTARGET      (1 << 16)
#define FL_AIMTARGET     (1 << 17)  // set if the crosshair needs to aim onto the entity
#define FL_PARTIALGROUND (1 << 18)  // not all corners are valid
#define FL_STATICPROP    (1 << 19)  // Eetsa static prop!
#ifdef PORTAL2
    #define FL_AFFECTED_BY_PAINT (1 << 20)
#else
    #define FL_GRAPHED (1 << 20)  // worldgraph has this ent listed as something that blocks a connection
#endif
#define FL_GRENADE               (1 << 21)
#define FL_STEPMOVEMENT          (1 << 22)  // Changes the SV_Movestep() behavior to not do any processing
#define FL_DONTTOUCH             (1 << 23)  // Doesn't generate touch functions, generates Untouch() for anything it was touching when this flag was set
#define FL_BASEVELOCITY          (1 << 24)  // Base velocity has been applied this frame (used to convert base velocity into momentum)
#define FL_WORLDBRUSH            (1 << 25)  // Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
#define FL_OBJECT                (1 << 26)  // Terrible name. This is an object that NPCs should see. Missiles, for example.
#define FL_KILLME                (1 << 27)  // This entity is marked for death -- will be freed by game DLL
#define FL_ONFIRE                (1 << 28)  // You know...
#define FL_DISSOLVING            (1 << 29)  // We're dissolving!
#define FL_TRANSRAGDOLL          (1 << 30)  // In the process of turning into a client side ragdoll.
#define FL_UNBLOCKABLE_BY_PLAYER (1 << 31)  // pusher that can't be blocked by the player

#define IN_ATTACK    (1 << 0)
#define IN_JUMP      (1 << 1)
#define IN_DUCK      (1 << 2)
#define IN_FORWARD   (1 << 3)
#define IN_BACK      (1 << 4)
#define IN_USE       (1 << 5)
#define IN_CANCEL    (1 << 6)
#define IN_LEFT      (1 << 7)
#define IN_RIGHT     (1 << 8)
#define IN_MOVELEFT  (1 << 9)
#define IN_MOVERIGHT (1 << 10)
#define IN_ATTACK2   (1 << 11)
#define IN_RUN       (1 << 12)
#define IN_RELOAD    (1 << 13)
#define IN_ALT1      (1 << 14)
#define IN_ALT2      (1 << 15)
#define IN_SCORE     (1 << 16)  // Used by client.dll for when scoreboard is held down
#define IN_SPEED     (1 << 17)  // Player is holding the speed key
#define IN_WALK      (1 << 18)  // Player holding walk key
#define IN_ZOOM      (1 << 19)  // Zoom key for HUD zoom
#define IN_WEAPON1   (1 << 20)  // weapon defines these bits
#define IN_WEAPON2   (1 << 21)  // weapon defines these bits
#define IN_BULLRUSH  (1 << 22)
#define IN_GRENADE1  (1 << 23)  // grenade 1
#define IN_GRENADE2  (1 << 24)  // grenade 2
#define IN_LOOKSPIN  (1 << 25)

// edict->movetype values
enum {
    MOVETYPE_NONE = 0,    // never moves
    MOVETYPE_ISOMETRIC,   // For players -- in TF2 commander view, etc.
    MOVETYPE_WALK,        // Player only - moving on the ground
    MOVETYPE_STEP,        // gravity, special edge handling -- monsters use this
    MOVETYPE_FLY,         // No gravity, but still collides with stuff
    MOVETYPE_FLYGRAVITY,  // flies through the air + is affected by gravity
    MOVETYPE_VPHYSICS,    // uses VPHYSICS for simulation
    MOVETYPE_PUSH,        // no clip to world, push and crush
    MOVETYPE_NOCLIP,      // No gravity, no collisions, still do velocity/avelocity
    MOVETYPE_LADDER,      // Used by players only when going onto a ladder
    MOVETYPE_OBSERVER,    // Observer movement, depends on player's observer mode
    MOVETYPE_CUSTOM,      // Allows the entity to describe its own physics

    // should always be defined as the last item in the list
    MOVETYPE_LAST = MOVETYPE_CUSTOM,

    MOVETYPE_MAX_BITS = 4
};

enum {
    FRAME_UNDEFINED = -1,  // (haven't run any frames yet)
    FRAME_START,

    // A network packet is being recieved
    FRAME_NET_UPDATE_START,
    // Data has been received and we're going to start calling PostDataUpdate
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    // Data has been received and we've called PostDataUpdate on all data recipients
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    // We've received all packets, we can now do interpolation, prediction, etc..
    FRAME_NET_UPDATE_END,

    // We're about to start rendering the scene
    FRAME_RENDER_START,
    // We've finished rendering the scene.
    FRAME_RENDER_END,

    FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};

#define TEXTURE_GROUP_LIGHTMAP       "Lightmaps"
#define TEXTURE_GROUP_WORLD          "World textures"
#define TEXTURE_GROUP_MODEL          "Model textures"
#define TEXTURE_GROUP_STATIC_PROP    "StaticProp textures"
#define TEXTURE_GROUP_COMBINED       "Combined Textures"
#define TEXTURE_GROUP_COMPOSITE      "Composited Textures"
#define TEXTURE_GROUP_VGUI           "VGUI textures"
#define TEXTURE_GROUP_PARTICLE       "Particle textures"
#define TEXTURE_GROUP_DECAL          "Decal textures"
#define TEXTURE_GROUP_SKYBOX         "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS "ClientEffect textures"
#define TEXTURE_GROUP_OTHER          "Other textures"
#define TEXTURE_GROUP_PRECACHED      "Precached"  // TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP       "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET  "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED    "Unaccounted textures"  // Textures that weren't assigned a texture group.
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		"Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER         "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP   "Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR  "Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD  "World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS "Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER  "Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER        "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER       "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER                "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL                  "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS               "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS              "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE       "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS               "Morph Targets"
#define TEXTURE_GROUP_SCALEFORM                   "Scaleform textures"