/*
 * GstVideoServer.h
 *
 *  Created on: 08.10.2011
 *      Author: babah
 */

#ifndef GSTVIDEOSERVER_H_
#define GSTVIDEOSERVER_H_

#include "GstPlayerBase.h"
#include <string>

using namespace std;

class GstVideoServer : public GstPlayerBase
{
	string m_HostIp;
	unsigned int m_Port;
	string m_DeviceName;
	Glib::RefPtr<Gst::Element> source, encoder, rtpenc, sink;

public:
	void Init();
	void Broadcast();
	void SetDevice(const string& deviceName) { m_DeviceName = deviceName; }
	void SetHost(const string& host) { m_HostIp = host; }
	void SetPort(unsigned int port) { m_Port = port;}
	GstVideoServer(const string& hostIP = "127.0.0.1", unsigned int port = 5000, const string& deviceName = "");
	virtual ~GstVideoServer();
};

#endif /* GSTVIDEOSERVER_H_ */
