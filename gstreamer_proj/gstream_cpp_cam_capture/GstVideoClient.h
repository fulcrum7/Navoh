/*
 * GstVideoClient.h
 *
 *  Created on: 08.10.2011
 *      Author: babah
 */

#ifndef GSTVIDEOCLIENT_H_
#define GSTVIDEOCLIENT_H_

#include "GstPlayerBase.h"
#include <gdkmm.h>
#include <boost/shared_array.hpp>

class GstVideoClient : public GstPlayerBase
{
	unsigned int m_Port;
	boost::shared_array<unsigned char> m_Frame_data_copy;
	unsigned int m_Frame_buffer_size;

	Glib::RefPtr<Gst::Element> source, decoder, rtpdec, capsfilter, ffmpegcolorspace1, ffmpegcolorspace2, sink;

public:
	void Init();
	void SetPort(unsigned int port);
	GstVideoClient(unsigned int port = 5000);
	virtual ~GstVideoClient();

	void on_handoff(const Glib::RefPtr<Gst::Buffer>& buf, const Glib::RefPtr<Gst::Pad>& pad);
	void get_frame(Glib::RefPtr<Gdk::Pixbuf>& bufpix);
};

#endif /* GSTVIDEOCLIENT_H_ */
