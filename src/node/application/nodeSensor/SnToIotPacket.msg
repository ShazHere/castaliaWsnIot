//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//
// TODO generated message class
//
// Description: Used to send informatin about SN towards IoT object
cplusplus {{ 
#include "ApplicationPacket_m.h" 
}} 
class ApplicationPacket; 
struct snInfo { 
    unsigned short nodeID; //the ID of the Node
    double locX; // x-coordinate of the node 
    double locY; // y-coordinate of the node 
    double spentEnergy; // energy spent by the node
} 
packet SnToIotPacket extends ApplicationPacket {
    snInfo extraData; 
}