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
        auto v1_values = json["v1"];
        vertices[0] = glm::vec3( v1_values[0], v1_values[1], v1_values[2] );

        auto v2_values = json["v2"];
        vertices[1] = glm::vec3( v2_values[0], v2_values[1], v2_values[2] );

        auto v3_values = json["v3"];
        vertices[2] = glm::vec3( v3_values[0], v3_values[1], v3_values[2] );

        auto colour_values = json["colour"];
        colour = glm::vec3( colour_values[0], colour_values[1], colour_values[2] );
    }

    static ComponentRegistrar<TriangleComponent> registrar;
};