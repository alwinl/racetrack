/*
 * component_registry.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "point.h"
#include "transform.h"
#include "triangle.h"
#include "velocity.h"
#include "track.h"

ComponentRegistrar<PointComponent> PointComponent::registrar("Point");
ComponentRegistrar<TriangleComponent> TriangleComponent::registrar("Triangle");
ComponentRegistrar<TrackComponent> TrackComponent::registrar("Track");
ComponentRegistrar<Transform> Transform::registrar("Transform");
ComponentRegistrar<Velocity> Velocity::registrar("Velocity");

void force_component_registration() {}
