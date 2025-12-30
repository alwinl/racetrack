/*
 * mesh_component.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "component_registry.h"

struct MeshComponent
{
    enum class Topology { POINTS, LINES, LINE_STRIP, LINE_LOOP, TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN };

    Topology topology;
    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> colours;

    bool filled = true;

    void from_json( const nlohmann::json& json ) {}

    static ComponentRegistrar<MeshComponent> registrar;
};
