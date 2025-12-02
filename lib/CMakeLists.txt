#
# CMakeLists.txt Copyright 2024 Alwin Leerling dna.leerling@gmail.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
#

find_package( glfw3 )

add_library(
    racetrack_lib

    engine.cc
    world.cc

    rendering/system_renderer.cc
    rendering/shader.cc
    rendering/point_renderer.cc
    rendering/triangle_renderer.cc
    
    base_system.cc
    system_input.cc
)

target_link_libraries(racetrack_lib PRIVATE glad glfw)
target_include_directories(racetrack_lib INTERFACE ./)
target_include_directories(racetrack_lib PUBLIC rendering)