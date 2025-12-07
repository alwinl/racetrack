/*
 * componentstorage.h Copyright 2025 Alwin Leerling <dna.leerling@gmail.com>
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

#include <unordered_map>

#include "../entity.h"

template< typename T>
class ComponentStorage
{
public:
    T* add( Entity e, const T& component ) { components[e] = component; return &(components[e]); }
    void remove( Entity e ) { components.erase(e); }
    bool has(Entity e) const { return components.count(e); }

    T* get( Entity e ) { auto it = components.find(e); return it != components.end() ? &it->second : nullptr; }
    const T* get( Entity e ) const { auto it = components.find(e); return it != components.end() ? &it->second : nullptr; }
    std::unordered_map<Entity,T>& all() { return components; }
    const std::unordered_map<Entity,T>& all() const { return components; }

private:
    std::unordered_map<Entity,T> components;
};
