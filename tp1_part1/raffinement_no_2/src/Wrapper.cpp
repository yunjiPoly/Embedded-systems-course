///////////////////////////////////////////////////////////////////////////////
//
//	Wrapper.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "Wrapper.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Constructor
//
///////////////////////////////////////////////////////////////////////////////
Wrapper::Wrapper(sc_module_name zName)
: sc_module(zName)
{
	// Create a SC_THREAD due to while(1) for handshaking
	SC_THREAD(interface);
}

///////////////////////////////////////////////////////////////////////////////
//
//	Destructor
//
///////////////////////////////////////////////////////////////////////////////
Wrapper::~Wrapper()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//	Interface avec le module Bubble
//
///////////////////////////////////////////////////////////////////////////////
void Wrapper::interface(void)
{
	// Variables
	unsigned int addr, data;

	// Initialize handshaking at thread start
	this->data.write(0);
	this->ack.write(false);
	wait(this->clk->posedge_event());

	while (1) {
		// Wait an incoming request from Bubble
		do {
			wait(this->clk->posedge_event());
		} while (!this->request.read());

		// Read address
		addr = this->address.read();

		// Ask DataRAM the data at addr  
		data = this->dataPortRAM->Read(addr);

		// Send value
		this->data.write(data);

		// Send an ACK
		this->ack.write(true);

		// Wait a cycle
		wait(this->clk->posedge_event());

		// Remove ACK
		this->ack.write(false);
	}
}
