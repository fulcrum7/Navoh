
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
#include <gtkmm/drawingarea.h>
#include <gdk/gdkpixbuf.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/box.h>
#include <pangomm.h>
#include <cairomm/cairomm.h>
#include <gstreamermm.h>
#include "GstVideoClient.h"
#include <dmtx.h>
#include <iostream>
#include <vector>
#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace std;
using namespace boost;

class VideoWidget : public Gtk::DrawingArea
{
	GstVideoClient vc;
	Glib::RefPtr<Gdk::Pixbuf> pixbuf;

public:
	VideoWidget()
	{
		vc.Init();
		vc.Play();
		vc.get_frame(pixbuf);
		set_size_request(pixbuf->get_width(), pixbuf->get_height());
	}
	virtual ~VideoWidget(){}

	virtual bool on_expose_event(GdkEventExpose* )
	{
		Glib::RefPtr<Gdk::Drawable> drawable = get_window();

		vc.get_frame(pixbuf);

		drawable->draw_pixbuf(get_style()->get_bg_gc(Gtk::STATE_NORMAL), pixbuf,
				0, 0, 0, 0, -1 , -1, Gdk::RGB_DITHER_NONE, 0, 0);

		Gdk::Point rect[4], barcode_center;
		string str;

		bool res = decode_barcode(pixbuf, rect, str, 50);

		if (res)
		{
			Cairo::RefPtr<Cairo::Context> cr = drawable->create_cairo_context();

			cr->set_line_width(3);
			cr->set_source_rgb(1, 0.0, 0.0);

			cr->move_to(rect[0].get_x(), rect[0].get_y());
			cr->line_to(rect[1].get_x(), rect[1].get_y());
			cr->line_to(rect[2].get_x(), rect[2].get_y());
			cr->line_to(rect[3].get_x(), rect[3].get_y());
			cr->line_to(rect[0].get_x(), rect[0].get_y());
			cr->stroke();

			Glib::RefPtr<Pango::Layout> pango_layout = Pango::Layout::create(cr);

			Pango::FontDescription desc("Arial Rounded MT Bold");
			desc.set_size(14 * PANGO_SCALE);

			pango_layout->set_font_description(desc);
			pango_layout->set_text(str);

			Gdk::Point* max_x, *min_x, *max_y, *min_y;
			boost::function<bool (Gdk::Point, Gdk::Point)> compare;

			compare = bind(less<int>(), bind(&Gdk::Point::get_x, _1), bind(&Gdk::Point::get_x, _2));
			max_x = max_element(rect, rect + 4, compare);
			min_x =	min_element(rect, rect + 4, compare);

			compare = bind(less<int>(), bind(&Gdk::Point::get_y, _1), bind(&Gdk::Point::get_y, _2));
			max_y = max_element(rect, rect + 4, compare);
			min_y =	min_element(rect, rect + 4, compare);

			barcode_center = Gdk::Point(
					(max_x->get_x() + min_x->get_x()) / 2,
					(max_y->get_y() + min_y->get_y()) / 2);


			const Pango::Rectangle extent = pango_layout->get_pixel_logical_extents();

			cr->move_to(barcode_center.get_x() - extent.get_width() / 2,
					barcode_center.get_y() - extent.get_height() / 2);

			pango_layout->show_in_cairo_context(cr);

//			cr->set_source_rgba(0, 0, 0, 0.6);
//
//			int i = 0;
//			if (center_barcode.get_x() < get_width() / 3)
//				i = 0;
//			else if (center_barcode.get_x() > (2 * get_width() / 3))
//				i = 2;
//			else
//				i = 1;
//
//			cr->rectangle(i * get_width() / 3,  0, i * get_width() / 3 + get_width() / 3, get_height());
//			cr->fill_preserve();

		}

//		cr->set_source_rgba(0, 0, 0, 0.6);
//		cr->move_to(get_width() / 3, 0);
//		cr->line_to(get_width() / 3, get_height());
//		cr->move_to(2 * get_width() / 3, 0);
//		cr->line_to(2 * get_width() / 3, get_height());
//		cr->stroke();

		return true;
	}

	bool decode_barcode(Glib::RefPtr<Gdk::Pixbuf>& pixbuf, Gdk::Point rect[4], string& str, long timeout_msec = 0)
	{
		DmtxImage* img = dmtxImageCreate(pixbuf->get_pixels(), pixbuf->get_width(), pixbuf->get_height(), DmtxPack24bppRGB);
		bool ret = false;
		if (img)
		{
		    DmtxVector2 p00, p10, p11, p01;
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
						str.assign(msg->output, msg->output + msg->outputSize);
						std::cout << "Output: " << msg->output << " Output index: " << msg->outputIdx << '\n';
						dmtxMessageDestroy(&msg);
						ret = true;
					}
					p00.X = p00.Y = p10.Y = p01.X = 0.0;
					p10.X = p01.Y = p11.X = p11.Y = 1.0;

					dmtxMatrix3VMultiplyBy(&p00, reg->fit2raw);
					dmtxMatrix3VMultiplyBy(&p10, reg->fit2raw);
					dmtxMatrix3VMultiplyBy(&p11, reg->fit2raw);
					dmtxMatrix3VMultiplyBy(&p01, reg->fit2raw);

					rect[0] = Gdk::Point((unsigned int)(p00.X + 0.5),
						(unsigned int)(get_height() - 1 - (int)(p00.Y + 0.5)));
					rect[1] = Gdk::Point((unsigned int)(p01.X + 0.5),
						(unsigned int)(get_height() - 1 - (int)(p01.Y + 0.5)));
					rect[2] = Gdk::Point((unsigned int)(p11.X + 0.5),
						(unsigned int)(get_height() - 1 - (int)(p11.Y + 0.5)));
					rect[3] = Gdk::Point((unsigned int)(p10.X + 0.5),
						(unsigned int)(get_height() - 1 - (int)(p10.Y + 0.5)));

					dmtxRegionDestroy(&reg);
				}
				dmtxDecodeDestroy(&dec);
			}
			dmtxImageDestroy(&img);
		}
		return ret;
	}
};


class HelloWorld : public Gtk::Window
{
public:
	HelloWorld();
	virtual ~HelloWorld(){};

protected:
	void on_button_clicked();
	bool on_timer_update_image();

	void decode_barcode(Glib::RefPtr<Gdk::Pixbuf>& pixbuf, long timeout_msec);


	VideoWidget m_VW;
	Gtk::Button m_Button;
	Gtk::VBox m_VBox;
};

HelloWorld::HelloWorld()
	: m_Button("Refresh images")
{
	set_border_width(10);

	Glib::RefPtr<Gdk::Pixbuf> pixbuf;

	m_VBox.pack_start(m_VW);
	m_VBox.pack_start(m_Button);

	Glib::signal_timeout().connect(sigc::mem_fun(this, &HelloWorld::on_timer_update_image), 67);
	m_Button.signal_clicked().connect(sigc::mem_fun(*this, &HelloWorld::on_button_clicked));

	add(m_VBox);
	show_all_children();
}

bool HelloWorld::on_timer_update_image()
{
    Glib::RefPtr<Gdk::Window> win = get_window();
    if (win)
    {
        Gdk::Rectangle r(0, 0, m_VW.get_width(), m_VW.get_height());
        win->invalidate_rect(r, true);
    }
	return true;
}

void HelloWorld::on_button_clicked()
{
	cout << "on_button_click" << endl;
	cout << m_VW.get_width() << " " << m_VW.get_height() << endl;
}

int main(int argc, char **argv)
{
	Gst::init();
	Gtk::Main kit(argc, argv);

	HelloWorld h;
	Gtk::Main::run(h);

	return 0;
}
