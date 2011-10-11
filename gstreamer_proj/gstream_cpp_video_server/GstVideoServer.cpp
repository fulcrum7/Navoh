/*
 * GstVideoServer.cpp
 *
 *  Created on: 08.10.2011
 *      Author: babah
 */

#include "GstVideoServer.h"

GstVideoServer::GstVideoServer(const string& hostIP,
	unsigned int port, const string& deviceName) :
	m_HostIp(hostIP), m_Port(port), m_DeviceName(deviceName)
{
}

GstVideoServer::~GstVideoServer()
{
	pipeline->set_state(Gst::STATE_NULL);
}

void GstVideoServer::Broadcast()
{
	Play();
}


void GstVideoServer::Init()
{
	using namespace Gst;
/*
 * gst-launch-0.10 -v v4l2src ! ffenc_mpeg4 bitrate=8388608 !\
 *		rtpmp4vpay mtu=1400 pt=96 ssrc=0 timestamp-offset=0 seqnum-offset=0 send-config=true !\
 *		udpsink host=127.0.0.1 port=5000
 */

	pipeline = Pipeline::create("video-server");

	source = ElementFactory::create_element("v4l2src");
	encoder = ElementFactory::create_element("ffenc_mpeg4");
	rtpenc = ElementFactory::create_element("rtpmp4vpay");
	sink = ElementFactory::create_element("udpsink");

	if (!pipeline || !source || !encoder || !rtpenc || !sink)
		throw exception();

	if (!m_DeviceName.empty())
		source->set_property("device", m_DeviceName);

	encoder->set_property("bitrate", 8388608);

	rtpenc->set_property("mtu", 1400);
	rtpenc->set_property("pt", 96);
	rtpenc->set_property("ssrc", 0);
	rtpenc->set_property("timestamp-offset", 0);
	rtpenc->set_property("seqnum-offset", 0);
	rtpenc->set_property("send-config", true);

	sink->set_property("host", m_HostIp);
	sink->set_property("port", m_Port);

	Glib::RefPtr<Gst::Bus> bus = pipeline->get_bus();
	bus->add_watch(sigc::mem_fun(dynamic_cast<GstPlayerBase&>(*this), &GstPlayerBase::on_bus_Message));

	pipeline->add(source)->add(encoder)->add(rtpenc)->add(sink);

	source->link(encoder)->link(rtpenc)->link(sink);
}



