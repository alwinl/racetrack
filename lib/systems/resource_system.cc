 /*
  * resourcesystem.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "resource_system.h"

#include <fstream>
#include "nlohmann/json.hpp"

#include "../world.h"
#include "../engine.h"
#include "../components/component_registry.h"

#include "../components/load_request_component.h"

void ResourceSystem::update( World &world, double dt )
{
    auto& load_requests = world.storage<LoadRequestComponent>();

    for( auto& [entity, event] : load_requests.all() ) {
        load( world, event.filename );
        world.remove_entity<LoadRequestComponent>( entity );
    }
}

void ResourceSystem::load( World& world, const std::string &filename )
{
    std::ifstream datafile( filename );

    if( !datafile.is_open() )
        return;

    nlohmann::json data;
    datafile >> data;

    for( auto& ent : data["entities"] ) {

        Entity e = world.create_entity();

        for( auto [name, component_data] : ent["components"].items() )
            ComponentRegistry::create( name, world, e, component_data );
    }
}

void ResourceSystem::flush( World& world )
{
    world.flush_components<LoadRequestComponent>();
}
