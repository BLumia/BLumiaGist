#include <gtkmm.h>
#include <iostream>
#include <iomanip> //setprecision
//#include <ctime> 
#include <cmath> //celi
#include <json-glib/json-glib.h>
#include "VPSUsage.hpp"

using namespace Glib;
using namespace std;

Gtk::Window* pDialog = 0;
Glib::RefPtr<Gtk::Builder> refBuilder;
VPSState myvps;

void updateBandwidthUsedInfo() {
	Gtk::ProgressBar* pProgressbar;
	Gtk::Label* pLabel;
	refBuilder->get_widget("bandwidthLeftProgressBar", pProgressbar);
	refBuilder->get_widget("bandwidthUsedLabel", pLabel);
	pProgressbar->set_fraction(myvps.getLeftProportion());
	ustring text = ustring::format(std::fixed, setprecision(3), myvps.getBandwidthLeftGiB());
	pProgressbar->set_text(text+" GiB Left");
	text = ustring::format(std::fixed, setprecision(2), myvps.getBandwidthUsedGiB());
	pLabel->set_markup("<span font_desc='32'>"+text+"</span> GiB");
}

void updateTimeLeftInfo() {
	Gtk::ProgressBar* pProgressbar;
	Gtk::Label* pLabel;
	refBuilder->get_widget("timeLeftProgressBar", pProgressbar);
	refBuilder->get_widget("timeLeftLabel", pLabel);
	pProgressbar->set_fraction(myvps.getTimeLeftProportion());
	pProgressbar->set_text(myvps.getClearTimeStr());
	ustring text = ustring::compose("<span font_desc='32'>%1</span> %2", myvps.getTimeLeftDay(), myvps.getTimeLeftDay()<=1 ? "Day" : "Days");
	pLabel->set_markup(text);
}

static void on_button_clicked()
{

    //g_type_init();
    JsonParser *parser = json_parser_new();
    gboolean ret;
    ret = json_parser_load_from_file(parser, "testUsage.json", NULL);
    if (!ret)
    {
		g_print ("Error\n");
	}
    JsonNode *root = json_node_new(JSON_NODE_OBJECT);
    root = json_parser_get_root(parser);
    JsonObject *obj = json_object_new();
    obj = json_node_get_object(root);
    // 以下可对obj进行get操作
    // gint64 %li 
	myvps.bandwidthTotalByte = json_object_get_int_member(obj,"plan_monthly_data"); // Total Bandwidth
	myvps.bandwidthUsedByte = json_object_get_int_member(obj,"data_counter"); // Used Bandwidth
    myvps.nextClearTimeUNIX = json_object_get_int_member(obj,"data_next_reset"); // Next bandwidth reset time
    
	g_print ("%d\n",myvps.getTimeLeftDay());
	updateBandwidthUsedInfo();
	updateTimeLeftInfo();
  //if(pDialog)
    //pDialog->hide(); //hide() will cause main::run() to end.
}

int main (int argc, char **argv)
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "net.blumia.vpsusage");

  //Load the GtkBuilder file and instantiate its widgets:
  refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("VPSUsageMain.glade");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  //Get the GtkBuilder-instantiated Dialog:
  refBuilder->get_widget("window1", pDialog);
  if(pDialog)
  {
    //Get the GtkBuilder-instantiated Button, and connect a signal handler:
    Gtk::Button* pButton = 0;
    refBuilder->get_widget("button1", pButton);
    if(pButton)
    {
      pButton->signal_clicked().connect( sigc::ptr_fun(on_button_clicked) );
    }

    app->run(*pDialog);
  }

  delete pDialog;

  return 0;
}
