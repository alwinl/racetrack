/*
 * geometry_component.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include <vector>

#include <glm/glm.hpp>
#include "nlohmann/json.hpp"

#include "../core/registry.h"

struct GeometryComponent
{
    float axis_a;
    float axis_b;
    int segments;
    bool closed;
    bool filled;
    glm::vec3 colour;

    bool generated = true;

    void from_json( const nlohmann::json& json )
    {
        axis_a = json.value( "axis_a", 1.0f );
        axis_b = json.value( "axis_b", 1.0f );
        segments = json.value( "segments", 4.0f );
        closed = json.value( "closed", true );
        filled = json.value( "filled", true );

        auto [r,g,b] = json.value( "colour", std::array<float,3> {1.0f, 0.0f, 0.0f} );
        colour = glm::vec3( r, g, b );

        generated = false;
    }
};