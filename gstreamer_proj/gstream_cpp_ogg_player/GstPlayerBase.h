/*
 * GstPlayerBase1.h
 *
 *  Created on: 08.10.2011
 *      Author: babah
 */

#ifndef GSTPLAYERBASE1_H_
#define GSTPLAYERBASE1_H_

#include <gstreamermm.h>

class GstPlayerBase
{
public:
	virtual void Init() = 0;
	virtual void Play();
	virtual void Stop();
	virtual ~GstPlayerBase(){};

	bool on_bus_Message(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message);
protected:
	Glib::RefPtr<Gst::Pipeline> pipeline;
};


#endif /* GSTPLAYERBASE1_H_ */
