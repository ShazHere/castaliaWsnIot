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

package node.application.nodeSensor;

simple NodeSensor like node.application.iApplication {
 parameters:
 	string applicationID = default ("wsnIot");//("nodeSensor"); //make it empty string for accepting everyapp packet
	bool collectTraceInfo = default (false);
	int packetHeaderOverhead = default (8);
	int priority = default (1);
	int constantDataPayload = default (8);

//application specific parameters, not defined in iApplication
	//inspired by connectivityMap app
	//int packetSpacing = default (100);	// in ms
	//int packetsPerNode = default (100);
	int packetSize = default (32);	// in bytes

//inspired by throughtput app
	double packet_rate = default (0);	// packets per second, by default we transmit no packets
	double startupDelay = default (0);	// delay in seconds before the app stars producing packets

 gates:
 	output toCommunicationModule;
	output toSensorDeviceManager;
	input fromCommunicationModule;
	input fromSensorDeviceManager;
	input fromResourceManager;
}