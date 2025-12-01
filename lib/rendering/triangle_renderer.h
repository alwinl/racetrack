/*
 * triangle_renderer.h Copyright 2025 Alwin Leerling <dna.leerling@gmail.com>
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

#include "base_renderer.h"
#include "../world.h"
#include "shader.h"

class TriangleRenderer : public BaseRenderer
{
public:
    void init();

    void destroy() override;
    void upload( const World& world ) override;
    void draw() override;

private:
    Shader shader;
    unsigned vao = 0;
    unsigned vbo = 0;

    glm::vec3 vertices[3];

    std::vector<glm::vec3> cpu_buffer;
};
