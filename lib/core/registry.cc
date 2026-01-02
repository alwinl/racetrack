/*
 * registry.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "../components/point_component.h"
#include "../components/transform_component.h"
#include "../components/triangle_component.h"
#include "../components/velocity_component.h"
#include "../components/track_component.h"
#include "../components/lake_component.h"
#include "../components/load_request_component.h"
#include "../components/mesh_component.h"
#include "../components/geometry_component.h"

ComponentRegistrar<PointComponent> PointComponent::registrar("Point");
ComponentRegistrar<TriangleComponent> TriangleComponent::registrar("Triangle");
ComponentRegistrar<TrackComponent> TrackComponent::registrar("Track");
ComponentRegistrar<LakeComponent> LakeComponent::registrar("Lake");
ComponentRegistrar<TransformComponent> TransformComponent::registrar("Transform");
ComponentRegistrar<VelocityComponent> VelocityComponent::registrar("Velocity");
ComponentRegistrar<LoadRequestComponent> LoadRequestComponent::registrar("Level");
ComponentRegistrar<MeshComponent> MeshComponent::registrar("mesh");
ComponentRegistrar<GeometryComponent> GeometryComponent::registrar("polygon");

void ComponentRegistry::register_funcs( const std::string& name, std::type_index t, ComponentCreator creator, ComponentDestroyer destroyer, ComponentFlusher flusher )
{
    get().creators.insert({name, creator});
    get().destroyers.insert({t, destroyer});
    get().flushers.insert({t, flusher});

    get().type_lookup.insert({name, t});
}

bool ComponentRegistry::create( World& world, Entity e, const std::string& name, const nlohmann::json& data)
{
    auto& creators_map = get().creators;

    auto it = creators_map.find(name);
    if( it == creators_map.end() )
        return false;

    auto& types_lookup = get().type_lookup;

    auto it2 = types_lookup.find( name );
    if( it2 == types_lookup.end() )
        return false;

    {
        auto [_,creator] = *it;
        creator( world, e, data );
    }
    {
        auto [_,type] = *it2;
        get().entity_typelist[e].push_back( type );
    }

    return true;
}

bool ComponentRegistry::destroy( World& world, Entity e )
{
    auto& typelist_map = get().entity_typelist;

    auto it = typelist_map.find(e);
    if( it == typelist_map.end() )
        return false;

    auto [_,type_list] = *it;

    auto& destroyers_map = get().destroyers;

    for( auto& type : type_list ) {

        auto it = destroyers_map.find( type );
        if( it == destroyers_map.end() )
            continue;

        auto [_,destroyer] = *it;

        destroyer( world, e );
    }

    typelist_map.erase( it );
    return true;
}

bool ComponentRegistry::flush_all( World& world )
{
    for( auto [_, flush_type] : get().flushers )
        flush_type( world );

    return true;
}

void ComponentRegistry::clear_all( World& world )
{
    auto& typelist = get().entity_typelist;

    // We copy keys because destroy() mutates the typelist map
    std::vector<Entity> to_delete;
    to_delete.reserve( typelist.size() );

    for( auto& [entity, types] : typelist )
        to_delete.push_back(entity);

    for( Entity e : to_delete )
        get().destroy( world, e );

    get().flush_all(world);
}
