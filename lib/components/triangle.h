/*
 * component_triangle.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include <glm/glm.hpp>

 #include "component_registry.h"

struct TriangleComponent
{
    glm::vec3 vertices[3] {
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}
    };
    glm::vec3 colour {1.0f};

    void from_json( const nlohmann::json& json )
    {
        std::array< std::array<float,3>, 3> defaults = {{
            {{-1.0f, 0.0f, 0.0f}},
            {{0.0f, 1.0f, 0.0f}},
            {{1.0f, 0.0f, 0.0f}}
        }};

        for( int i=0; i < 3; ++i ) {
            auto [vx,vy,vz] = json.value( std::string( "v" + std::to_string(i+1) ).c_str(), defaults[i] );
            vertices[i] = glm::vec3( vx,vy,vz );
        }

        auto [r,g,b] = json.value( "colour", std::array<float,3> {1.0f, 0.0f, 0.0f} );
        colour = glm::vec3( r, g, b );
    }

    static ComponentRegistrar<TriangleComponent> registrar;
};