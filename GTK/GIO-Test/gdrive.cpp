// g++ -std=c++11 `pkg-config --cflags --libs gtkmm-3.0` -o test gdrive.cpp

#include <iostream>

#include <glibmm.h>
#include <giomm.h>

#include <giomm/volumemonitor.h>

using namespace Gio;

int main() {
	Glib::init();
	Gio::init();

	Glib::RefPtr<VolumeMonitor> vm = Gio::VolumeMonitor::get();
	Glib::ListHandle<Glib::RefPtr<Drive>> drv = vm->get_connected_drives();
	for (const Glib::RefPtr<Drive> oneDrv : drv) {
		std::cout << oneDrv->get_name() << std::endl;
		std::cout << " - can_stop: " << oneDrv->can_stop() << std::endl 
			  << " - is_removable: " << oneDrv->is_removable() << std::endl;
	}	
}
