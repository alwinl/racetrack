/*
 * world.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "world.h"

void World::init()
{
    // --- Create a sample entity ---
    auto e = create_entity();

    auto& t = add_transform(e);
    t.x = 0.f;
    t.y = 0.f;
    t.z = 0.f;

    auto& v = add_velocity(e);
    v.dx = 1.f;
    v.dy = 0.f;
    v.dz = 0.f;

    e = create_entity();

    auto& triangle = add_triangle(e);
    triangle.a = { -0.5f, -0.5f, 0.0f };
    triangle.b = {  0.5f, -0.5f, 0.0f };
    triangle.c = {  0.0f,  0.5f, 0.0f };

}

void World::update( double dt )
{
    // Move all entities that have both Transform and Velocity
    for( auto& [entity, t] : transforms ) {

        if( velocities.count(entity) ) {
            auto& v = velocities[entity];
            t.x += v.dx * dt;
            t.y += v.dy * dt;
            t.z += v.dz * dt;
        }

    }
}

void World::cleanup()
{

}
