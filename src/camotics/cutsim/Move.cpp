/******************************************************************************\

    CAMotics is an Open-Source simulation and CAM software.
    Copyright (C) 2011-2015 Joseph Coffland <joseph@cauldrondevelopment.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

\******************************************************************************/

#include "Move.h"

#include <cbang/Math.h>
#include <cbang/log/Logger.h>

using namespace CAMotics;
using namespace cb;
using namespace std;


Move::Move(MoveType type, const Axes &start, const Axes &end, real startTime,
           int tool, real feed, real speed, unsigned line) :
  Segment3R(start.getXYZ(), end.getXYZ()), type(type),
  start(start), end(end), tool(tool), feed(feed), speed(speed), line(line),
  dist(start.distance(end)), time(feed ? dist / feed * 60 : 0),
  startTime(startTime) {

  // TODO fix rapid feed time

  if (type != MoveType::MOVE_RAPID && !feed)
    THROW("Cutting move with zero feed");
}


Color Move::getColor(int speed) const {
  switch (type) {
  case MoveType::MOVE_RAPID:   return Color::RED;
  case MoveType::MOVE_CUTTING: return Color(0, 1, 0, speed/12000);
  case MoveType::MOVE_PROBE:   return Color::BLUE;
  case MoveType::MOVE_DRILL:   return Color::YELLOW;
  }
  THROWS("Invalid move type " << type);
}


Vector3R Move::getPtAtTime(real time) const {
  if (getEndTime() <= time) return getEndPt();
  if (time <= getStartTime()) return getStartPt();

  real delta = time - getStartTime();
  return getStartPt() + (getEndPt() - getStartPt()) * delta / getTime();
}


void Move::print(std::ostream &stream) const {
  stream
    << "type:" << type << ' '
    << "x:" << end.getX() << ' '
    << "y:" << end.getY() << ' '
    << "z:" << end.getZ() << ' '
    << "tool:" << tool << ' '
    << "feed:" << feed << ' '
    << "speed:" << speed << ' '
    << "line:" << line;
}
