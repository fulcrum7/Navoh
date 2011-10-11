/*
 * main.cpp
 *
 *  Created on: 05.10.2011
 *      Author: babah
 */

#include <gstreamermm.h>
#include <iostream>
#include <string>

using namespace std;

class GstOggPlayer
{
	string m_Filename;
	Glib::RefPtr<Gst::Pipeline> pipeline;
	Glib::RefPtr<Gst::Element> source, parser, decoder, conv, sink;

public:
	void Init();
	void Play();
	void Stop();
	void SetFile(const string& fileName);
	GstOggPlayer();
	GstOggPlayer(const string& fileName);
	~GstOggPlayer();

private:
	void on_parser_pad_added(const Glib::RefPtr<Gst::Pad>& newPad);
	bool on_bus_Message(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message);
};

GstOggPlayer::GstOggPlayer()
{
//	if (!Gst::init_check())
//		Gst::init();
	cout << "Gst::init" << endl;
}

GstOggPlayer::GstOggPlayer(const string& fileName) : m_Filename(fileName)
{
//	if (!Gst::init_check())
//		Gst::init();
	cout << "Gst::init" << endl;
}

GstOggPlayer::~GstOggPlayer()
{
	pipeline->set_state(Gst::STATE_NULL);
}

void GstOggPlayer::Play()
{
	cout << "Setting playing" << endl;
	pipeline->set_state(Gst::STATE_PLAYING);
}

void GstOggPlayer::Stop()
{
	pipeline->set_state(Gst::STATE_NULL);
}

void GstOggPlayer::SetFile(const string& fileName)
{
	m_Filename = fileName;
}

void GstOggPlayer::Init()
{
	using namespace Gst;

	if (m_Filename.empty())
		throw exception();

	pipeline = Pipeline::create("audio-player");

	source = ElementFactory::create_element("filesrc");
	parser = ElementFactory::create_element("oggdemux");
	decoder = ElementFactory::create_element("vorbisdec");
	conv = ElementFactory::create_element("audioconvert");
	sink = ElementFactory::create_element("alsasink");

	if (!pipeline || !source || !parser || !decoder || !conv || !sink)
		throw exception();

	source->set_property("location", m_Filename);

	Glib::RefPtr<Gst::Bus> bus = pipeline->get_bus();
	bus->add_watch(sigc::mem_fun(*this, &GstOggPlayer::on_bus_Message));

	pipeline->add(source)->add(parser)->add(decoder)->add(conv)->add(sink);

	source->link(parser);
	parser->signal_pad_added().connect(
			sigc::mem_fun(*this, &GstOggPlayer::on_parser_pad_added));
	decoder->link(conv)->link(sink);
}

void GstOggPlayer::on_parser_pad_added(const Glib::RefPtr<Gst::Pad>& newPad)
{
	cout << "Dynamic pad created. Linking parser/decoder" << endl;
	Glib::RefPtr<Gst::Pad> sinkPad = decoder->get_static_pad("sink");
	Gst::PadLinkReturn ret = newPad->link(sinkPad);

	if (ret != Gst::PAD_LINK_OK && ret != Gst::PAD_LINK_WAS_LINKED)
	{
		std::cerr << "Linking of pads " << newPad->get_name() << " and " <<
				sinkPad->get_name() << " failed." << std::endl;
	}
}


bool GstOggPlayer::on_bus_Message(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message)
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


