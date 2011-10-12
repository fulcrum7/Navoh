/*
 * GstOggPlayer.cpp
 *
 *  Created on: 13.10.2011
 *      Author: babah
 */

#include "GstOggPlayer.h"
#include <iostream>

using namespace std;

GstOggPlayer::GstOggPlayer()
{
}

GstOggPlayer::GstOggPlayer(const string& fileName) : m_Filename(fileName)
{
}

GstOggPlayer::~GstOggPlayer()
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

	//bus->add_watch(sigc::mem_fun(*this, &GstPlayerBase::on_bus_Message));

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

