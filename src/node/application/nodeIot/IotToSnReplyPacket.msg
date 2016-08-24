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
cplusplus {{ 
#include "ApplicationPacket_m.h" 
}} 
class ApplicationPacket; 
struct iotInfo { 
    unsigned short nodeID; //the ID of the Node
    double locX; // x-coordinate of the node 
    double locY; // y-coordinate of the node 
    double speed;
    double broadcastPower; // so far I think its in dbm, and its same as broadcast range
    int direction; // so far I assume 1 means towards sink and 0 means far away. this is defined in 
    //line mobility module at get Direction(). LinemoblityModules uses 0 and 1 by default. 
} 
packet IotToSnReplyPacket extends ApplicationPacket {
    iotInfo extraData; 
}