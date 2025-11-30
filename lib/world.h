/*
 * world.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#pragma once

#include <cstdint>
#include <unordered_map>

#include "component_transform.h"
#include "component_velocity.h"


class World
{
public:
    using Entity = std::uint32_t;

    void init();
    void update( double dt );
    void cleanup();

    Entity create_entity() { return next_id++; }

    Transform& add_transform( Entity e ) { return transforms[e]; }
    Velocity& add_velocity( Entity e ) { return velocities[e]; }

    bool has_transform(Entity e) const { return transforms.count(e); }
    bool has_velocity(Entity e) const { return velocities.count(e); }

    const std::unordered_map<Entity, Transform>& get_transforms() const { return transforms; }
    const std::unordered_map<Entity, Velocity>& get_velocities() const { return velocities; }

private:
    Entity next_id = 1;

    std::unordered_map<Entity, Transform> transforms;
    std::unordered_map<Entity, Velocity> velocities;
};
