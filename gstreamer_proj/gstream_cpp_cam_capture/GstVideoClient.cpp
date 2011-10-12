/*
 * GstVideoClient.cpp
 *
 *  Created on: 08.10.2011
 *      Author: babah
 */

#include "GstVideoClient.h"
#include <gstreamermm.h>
#include <gst/gstcaps.h>
#include <gst/app/gstappsink.h>
#include <iostream>

using namespace std;

GstVideoClient::GstVideoClient(unsigned int port) :
		m_Port(port), m_Frame_data_copy(NULL), m_Frame_buffer_size(0)
{
}

GstVideoClient::~GstVideoClient()
{
	pipeline->set_state(Gst::STATE_NULL);
}


void GstVideoClient::SetPort(unsigned int port)
{
	m_Port = port;
}

void GstVideoClient::Init()
{
	using namespace Gst;
/*
 *  gst-launch-0.10 -v udpsrc port=5000 caps = "application/x-rtp, media=(string)video, clock-rate=(int)90000,
 *  encoding-name=(string)MP4V-ES, profile-level-id=(string)1,
 *  config=(string)000001b001000001b58913000001000000012000c48d8800f514043c1463000001b24c61766335322e32302e31,
 *  payload=(int)96, ssrc=(guint)0, clock-base=(guint)0, seqnum-base=(guint)0" !
 *  rtpmp4vdepay ! ffdec_mpeg4 ! ffmpegcolorspace !
 *  "video/x-raw-rgb, width=640, height=480, bpp=24, red_mask=255, green_mask=65280, blue_mask=16711680, endianness=4321" !
 *  ffmpegcolorspace ! appsink
 */

	pipeline = Pipeline::create("audio-player");

	//source = ElementFactory::create_element("v4l2src");
	source = ElementFactory::create_element("udpsrc");
	decoder = ElementFactory::create_element("ffdec_mpeg4");
	rtpdec = ElementFactory::create_element("rtpmp4vdepay");
	capsfilter = ElementFactory::create_element("capsfilter");
	ffmpegcolorspace1 = ElementFactory::create_element("ffmpegcolorspace");
	ffmpegcolorspace2 = ElementFactory::create_element("ffmpegcolorspace");
	sink = ElementFactory::create_element("appsink");

	//sink->set_property("emit-signals", true);
	sink->set_property("max-buffers", 2);
	sink->set_property("drop", true);

	//sink = ElementFactory::create_element("fakesink");

	//Glib::RefPtr<Gst::FakeSink>::cast_dynamic(sink)->property_signal_handoffs() = true;
	//Glib::RefPtr<Gst::FakeSink>::cast_dynamic(sink)->signal_handoff().connect(sigc::mem_fun(*this, &GstVideoClient::on_handoff));

	if (!pipeline || !source || !decoder || !rtpdec || !ffmpegcolorspace1 || !ffmpegcolorspace2 || !capsfilter || !sink)
		throw exception();

	source->set_property("port", m_Port);

	Glib::RefPtr<Gst::Caps> caps = Gst::Caps::create_simple("application/x-rtp");
	caps->set_simple("media", "video");
	caps->set_simple("clock-rate", 90000);
	caps->set_simple("encoding-name", "MP4V-ES");
	caps->set_simple("profile-level-id","1");
	caps->set_simple("config","000001b001000001b58913000001000000012000c48d8800f514043c1463000001b24c61766335322e32302e31");
	caps->set_simple("payload",96);
	caps->set_simple("ssrc", 0);
	caps->set_simple("clock-base", 0);
	caps->set_simple("seqnum-base", 0);

	source->set_property("caps", caps);

	Glib::RefPtr<Gst::Caps> rgbconvertcaps = Gst::Caps::create_simple("video/x-raw-rgb");
	rgbconvertcaps->set_simple("bpp", 24);
	rgbconvertcaps->set_simple("depth", 24);
//	rgbconvertcaps->set_simple("red_mask", 0xFF0000);
//	rgbconvertcaps->set_simple("green_mask", 0xFF00);//65280);
//	rgbconvertcaps->set_simple("blue_mask", 0xFF);//16711680);
	rgbconvertcaps->set_simple("endianness", 4321);

	capsfilter->set_property("caps", rgbconvertcaps);

	Glib::RefPtr<Gst::Bus> bus = pipeline->get_bus();
	bus->add_watch(sigc::mem_fun(dynamic_cast<GstPlayerBase&>(*this), &GstPlayerBase::on_bus_Message));

	pipeline->add(source)->add(rtpdec)->add(decoder)->add(ffmpegcolorspace1)->add(capsfilter)->add(ffmpegcolorspace2)->add(sink);

	source->link(rtpdec)->link(decoder)->link(ffmpegcolorspace1)->link(capsfilter)->link(ffmpegcolorspace2)->link(sink);
//	pipeline->add(source)->add(capsfilter)->add(ffmpegcolorspace1)->add(sink);
//	source->link(capsfilter)->link(ffmpegcolorspace1)->link(sink);
}

void GstVideoClient::get_frame(Glib::RefPtr<Gdk::Pixbuf>& pixbuf)
{
	int width = 0, height = 0;
	int rowstride;
	int bits_per_sample;
	bool has_alpha;
	unsigned char* data;
	GstBuffer* gbuf;
	unsigned int gbuf_size;
	GstCaps* caps;
	GstStructure* structure;

	gbuf = gst_app_sink_pull_buffer((GstAppSink*)sink->gobj());
	if (!gbuf) return;

	caps = gst_buffer_get_caps(gbuf);
	if (!caps) return;

	structure = gst_caps_get_structure(caps, 0);
	if (structure)
	{
		gst_structure_get_int(structure, "width", &width);
		gst_structure_get_int(structure, "height", &height);
	}

	gbuf_size = GST_BUFFER_SIZE(gbuf);
	if (m_Frame_buffer_size != gbuf_size)
	{
		m_Frame_buffer_size = gbuf_size;
		m_Frame_data_copy.reset(new unsigned char [m_Frame_buffer_size]);
	}

	data = GST_BUFFER_DATA(gbuf);

	copy(data, data + m_Frame_buffer_size, m_Frame_data_copy.get());

	rowstride = width * 3;
	bits_per_sample = 8;
	has_alpha = false;

	pixbuf = Gdk::Pixbuf::create_from_data(m_Frame_data_copy.get(), Gdk::COLORSPACE_RGB,
			has_alpha, bits_per_sample, width, height, rowstride);

	//std::cout << "timestamp: " << GST_BUFFER_TIMESTAMP(gbuf) << endl;
	gst_caps_unref(caps);
	gst_buffer_unref(gbuf);
}

void GstVideoClient::on_handoff(const Glib::RefPtr<Gst::Buffer>& buf, const Glib::RefPtr<Gst::Pad>& pad)
{
	int width = 0, height = 0;
	Glib::RefPtr<Gst::Caps> caps = buf->get_caps();
	const Gst::Structure structure = caps->get_structure(0);

	Gst::Fourcc format;
	if (structure)
	{
		structure.get_field("width", width);
		structure.get_field("height", height);
		structure.get_field("format", format);

	}
	std::cout << "on_handoff: widht=" << width << " height=" << height
			<< " format:" << format.first << format.second << format.third << format.fourth
			<< " timestamp=" << buf->get_timestamp() << " size=" << buf->get_size() << endl;
}
