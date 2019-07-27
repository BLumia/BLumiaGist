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

	std::cout << "--------------------" << std::endl;

	Glib::ListHandle<Glib::RefPtr<Volume>> vol  = vm->get_volumes();
	for (const Glib::RefPtr<Volume> oneVol : vol) {
		std::cout << oneVol->get_name() << std::endl;
		std::cout << " - uuid: " << oneVol->get_uuid() << std::endl;
	}

	std::cout << "--------------------" << std::endl;

	Glib::ListHandle<Glib::RefPtr<Mount>> mnt = vm->get_mounts();
	for (const Glib::RefPtr<Mount> oneMnt : mnt) {
		Glib::RefPtr<const File> file = oneMnt->get_root();
		std::cout << oneMnt->get_name() << std::endl;
		std::cout << " - uuid: " << oneMnt->get_uuid() << std::endl
			  << " - uri and path:" << file->get_uri() << " " << file->get_path() << std::endl
			  << " - default_location uri: " << oneMnt->get_default_location()->get_uri() << std::endl;
	}
}
