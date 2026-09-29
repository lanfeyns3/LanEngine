#pragma once

#include <box3d/box3d.h>
#include "core/LayerSystem.h"

#include <iostream>

namespace LANE
{
    class PhysicsManager
    {
    public:
        PhysicsManager() {
            b3WorldDef worldDef = b3DefaultWorldDef();

            worldDef.gravity = { 0.0f, -9.81f, 0.0f };
            
            worldID = b3CreateWorld(&worldDef);
        }

        b3WorldId& GetWorldID() { return worldID;}

        void UpdatePhysics(float deltaTime,LayerSystem& layers) {
            b3World_Step(worldID, deltaTime, 4);
            layers.UpdatePhysics(deltaTime);
        }

        bool IsPhysicsEnabled() {return runPhysics;}

        void EnablePhysics() {runPhysics = true;}
        void DisablePhysics() {runPhysics = false;}
    private:
        b3WorldId worldID;
        bool runPhysics = false;
    };
} // namespace LANE
