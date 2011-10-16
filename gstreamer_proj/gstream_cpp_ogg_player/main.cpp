/*
 * main.cpp
 *
 *  Created on: 05.10.2011
 *      Author: babah
 */

#include <gstreamermm.h>
#include <iostream>
#include <string>
#include "GstOggPlayer.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		cout << "Usage: " << argv[0] << "ogg file" << endl;
		return 1;
	}

	Gst::init();
	Glib::RefPtr<Glib::MainLoop> mainloop;
	mainloop = Glib::MainLoop::create();

	GstOggPlayer player1;
	player1.SetFile(argv[1]);
   	player1.Init();
	player1.Play();

	mainloop->run();
//	player1.Stop();
//	player1.SetFile("/home/babah/Рабочий стол/Hydrate-Kenny_Beltrey.ogg");
//	player1.Init();
//	player1.Play();

	return 0;
}


