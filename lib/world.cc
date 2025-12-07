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

#include "components/component_registry.h"

void World::clear_all_entities()
{
    // Ask the registry to destroy all components for all entities
    auto& typelist = ComponentRegistry::get_entity_typelist();

    // We copy keys because destroy() mutates the typelist map
    std::vector<Entity> to_delete;
    to_delete.reserve(typelist.size());
    for (auto& [entity, types] : typelist)
        to_delete.push_back(entity);

    // Destroy all components for each entity
    for (Entity e : to_delete)
        ComponentRegistry::destroy(*this, e);

    // Flush all component removals
    ComponentRegistry::flush_all(*this);

    // Reset entity ids (optional)
    next_id = 1;
}
