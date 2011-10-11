/*
 * main.cpp
 *
 *  Created on: 08.10.2011
 *      Author: babah
 */

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/main.h>
#include <gdk/gdkpixbuf.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/box.h>
#include <gstreamermm.h>
#include "GstVideoClient.h"
#include <dmtx.h>
#include <iostream>

class HelloWorld : public Gtk::Window
{
public:
	HelloWorld();
	virtual ~HelloWorld(){};

protected:
	void on_button_clicked();
	bool on_timer_update_image();

	void decode_barcode(Glib::RefPtr<Gdk::Pixbuf>& pixbuf, long timeout_msec);

	GstVideoClient vc;
	Gtk::Button m_Button;
	Gtk::Image m_Img;
	Gtk::VBox m_VBox;
};

HelloWorld::HelloWorld()
	: m_Button("Refresh images")
{
	vc.Init();
	vc.Play();
	set_border_width(10);

	Glib::RefPtr<Gdk::Pixbuf> pixbuf;
	vc.get_frame(pixbuf);
	m_Img.set(pixbuf);

	m_VBox.pack_start(m_Img);
	m_VBox.pack_start(m_Button);

	Glib::signal_timeout().connect(sigc::mem_fun(this, &HelloWorld::on_timer_update_image), 67);
	m_Button.signal_clicked().connect(sigc::mem_fun(*this, &HelloWorld::on_button_clicked));

	add(m_VBox);
	show_all_children();
}

void HelloWorld::decode_barcode(Glib::RefPtr<Gdk::Pixbuf>& pixbuf, long timeout_msec = 0)
{
	DmtxImage* img = dmtxImageCreate(pixbuf->get_pixels(), pixbuf->get_width(), pixbuf->get_height(), DmtxPack24bppRGB);

	if (img)
	{
		DmtxTime timeout;

		timeout = dmtxTimeAdd(dmtxTimeNow(), timeout_msec);
		DmtxDecode* dec = dmtxDecodeCreate(img, 1);
		if (dec)
		{
			DmtxRegion* reg = dmtxRegionFindNext(dec, &timeout);

			if (reg)
			{
				DmtxMessage *msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);
				if (msg)
				{
					std::cout << "Output: " << msg->output << " Output index: " << msg->outputIdx << '\n';
					dmtxMessageDestroy(&msg);
				}
				dmtxRegionDestroy(&reg);
			}
			dmtxDecodeDestroy(&dec);
		}
		dmtxImageDestroy(&img);
	}
}

bool HelloWorld::on_timer_update_image()
{
	Glib::RefPtr<Gdk::Pixbuf> pixbuf;
	vc.get_frame(pixbuf);
	m_Img.set(pixbuf);

	decode_barcode(pixbuf, 50);

	return true;
}

void HelloWorld::on_button_clicked()
{
	Glib::RefPtr<Gdk::Pixbuf> pixbuf;
	vc.get_frame(pixbuf);
	m_Img.set(pixbuf);

	std::cout << "on_button_click" << std::endl;
	decode_barcode(pixbuf, 5000);
}

int main(int argc, char **argv)
{
	Gst::init();
	Gtk::Main kit(argc, argv);

	HelloWorld h;
	Gtk::Main::run(h);

	return 0;
}
