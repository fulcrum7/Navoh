/*
 * main.cpp
 *
 *  Created on: 05.10.2011
 *      Author: babah
 */

#include <gstreamermm.h>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "GstVideoServer.h"

using namespace std;

struct compare_str : public std::binary_function<string,string,bool>
{
	result_type operator()(const first_argument_type& lhs, const second_argument_type& rhs) const
	{
		return equal(rhs.begin(), rhs.end(), lhs.begin());
	}
};

int main(int argc, char **argv)
{
	vector<string> param;
	vector<string>::iterator it_ip, it_port, it_device, it_help;
	string ip, device;
	unsigned int port;

	for (int i = 1; i < argc; ++i)
		param.push_back(argv[i]);

	it_ip = find_if(param.begin(), param.end(), bind2nd(compare_str(), "ip="));
	it_port = find_if(param.begin(), param.end(), bind2nd(compare_str(),"port="));
	it_device = find_if(param.begin(), param.end(), bind2nd(compare_str(), "device="));
	it_help = find_if(param.begin(), param.end(), bind2nd(equal_to<string>(), "--help"));

	if (it_help != param.end())
	{
		cout << "Usage:\n  gstream_cpp_test [ip=<host_address>] [port=value] [device=<path_to_camera>]\n" <<
			"Example:\n  gstream_cpp_video_test ip=192.168.1.33 port=5001 device=/dev/video1\n  "<<
			"By default uses ip=127.0.0.1 port=5000 device=<default v4l2 device>" << endl;
		return 0;
	}

	Gst::init();
	Glib::RefPtr<Glib::MainLoop> mainloop;
	mainloop = Glib::MainLoop::create();

	GstVideoServer player1;

	if (it_ip != param.end())
	{
		ip.assign(it_ip->begin() + 3, it_ip->end());
		player1.SetHost(ip);
	}
	if (it_port != param.end())
	{
		port = atoi(it_port->c_str() + 5);
		player1.SetPort(port);
	}
	if (it_device != param.end())
	{
		device.assign(it_device->begin() + 7, it_device->end());
		player1.SetDevice(device);
	}

	player1.Init();
	player1.Broadcast();

	mainloop->run();

	return 0;
}


