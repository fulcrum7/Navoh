/*
 * GstPlayerBase1.cpp
 *
 *  Created on: 08.10.2011
 *      Author: babah
 */

#include "GstPlayerBase.h"
#include <iostream>
using namespace std;

void GstPlayerBase::Play()
{
	cout << "Setting playing" << endl;
	pipeline->set_state(Gst::STATE_PLAYING);
}

void GstPlayerBase::Stop()
{
	pipeline->set_state(Gst::STATE_NULL);
}


bool GstPlayerBase::on_bus_Message(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message)
{
	switch (message->get_message_type())
	{
	case Gst::MESSAGE_EOS:
		Stop();
		break;
	case Gst::MESSAGE_ERROR:
		{
			Glib::RefPtr<Gst::MessageError> msgError = Glib::RefPtr<Gst::MessageError>::cast_dynamic(message);
			if(msgError)
			{
				Glib::Error err;
				err = msgError->parse();
				std::cerr << "Error: " << err.what() << std::endl;
			}
			else
				std::cerr << "Error." << std::endl;

		Stop();
		return false;
		}
	default:
		break;
	}
	return true;
}


