/*
 * shader.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include <string>

#include <glm/glm.hpp>

class Shader
{
public:
    void init( std::string vs_source, std::string fs_source );

    void activate();

	void set_uniform( const std::string &name, glm::mat4 matrix );

private:
    unsigned id;
    bool is_compiled = false;
    std::string vertex_source;
    std::string fragment_source;

    void compile( unsigned id, std::string source );
    void link( unsigned vs_id, unsigned fs_id );
};