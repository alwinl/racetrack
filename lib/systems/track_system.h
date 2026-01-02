/*
 * track_system.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "base_system.h"

#include "../core/world.h"

struct TrackComponent;

class TrackSystem : public BaseSystem<TrackSystem>
{
public:
    TrackSystem( Engine* eng ) : BaseSystem<TrackSystem>( eng ) {};

    void update( World& world, double dt ) override;

private:
    void regenerate_mesh( World& world, Entity ent, TrackComponent& track );
};