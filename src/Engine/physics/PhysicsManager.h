#pragma once

#include <box3d/box3d.h>
#include "core/LayerSystem.h"
#include "core/SceneManager.h"

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

        template<typename ViewFn, typename TransformFn, typename PhysicsFn>
        void ResetBodies(
            ViewFn getView,
            TransformFn getTransform,
            PhysicsFn getPhysics
        )
        {
            for (auto entity : getView())
            {
                auto& transform = getTransform(entity);
                auto& physics = getPhysics(entity);

                auto bTransform = b3Body_GetTransform(physics.id);
                bTransform.p = b3Vec3(transform.position.x,transform.position.y,transform.position.z);

                b3Body_SetTransform(physics.id,bTransform.p,bTransform.q);
            }
        }

        bool IsPhysicsEnabled() {return runPhysics;}

        void EnablePhysics() {runPhysics = true;}
        void DisablePhysics() {runPhysics = false;}
    private:
        b3WorldId worldID;
        bool runPhysics = false;
    };
} // namespace LANE
