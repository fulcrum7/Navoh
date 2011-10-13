/*
 * GstOggPlayer.h
 *
 *  Created on: 13.10.2011
 *      Author: babah
 */

#ifndef GSTOGGPLAYER_H_
#define GSTOGGPLAYER_H_

#include "GstPlayerBase.h"
#include <string>

class GstOggPlayer : public GstPlayerBase
{
	std::string m_Filename;
	Glib::RefPtr<Gst::Element> source, parser, decoder, conv, sink;

public:
	void Init();
	void SetFile(const std::string& fileName);
	GstOggPlayer();
	GstOggPlayer(const std::string& fileName);
	~GstOggPlayer();

private:
	void on_parser_pad_added(const Glib::RefPtr<Gst::Pad>& newPad);
};


#endif /* GSTOGGPLAYER_H_ */
