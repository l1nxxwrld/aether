#pragma once
#include <cstdint>
#include "../../math/vec3.hpp"
#include "../../math/vec4.hpp"
#include "../../math/qangle.hpp"

namespace aether::cs2 {
    enum class e_team_number : std::uint8_t {
        terrorist = 2,
        counter_terrorist = 3,
    };

    class CBodyComponentBaseAnimGraph;
    class CSkeletonInstance;
    class CCollisionProperty;

    class C_BaseEntity {
    public:
        CBodyComponentBaseAnimGraph* anim_graph() const;
        const char* get_entity_type_name() const;
        bool is_dormant() const;
        CCollisionProperty* collision_property() const;
        std::int32_t health() const;
        std::uint32_t flags() const;
        e_team_number team_number() const;
        std::int32_t entity_index() const;
    };

    // CBodyComponentBaseAnimGraph : CBodyComponentSkeletonInstance : CBodyComponent : CEntityComponent
    class CBodyComponentBaseAnimGraph {
    public:
        CSkeletonInstance* skeleton() const;
    };

    // NetworkVar_m_skeletonInstance@CBodyComponentSkeletonInstance : CSkeletonInstance : CGameSceneNode : ISceneAnimatableProceduralBoneTransforms
    class CSkeletonInstance {
    public:
        class bone {
        public:
            vec4 position;
            vec4 scale;
        };

        // 0x10: vec3 origin
        // 0x30: pawn

        std::int32_t bone_count() const;
        const bone& get_bone(std::int32_t index) const;
    };

    // C_BaseModelEntity::NetworkVar_m_Collision : CCollisionProperty
    class CCollisionProperty {
    public:
        const vec3& mins() const;
        const vec3& maxs() const;
    };
}
