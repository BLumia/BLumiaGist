#ifndef __BLUMIA_VPSUSAGE_HPP__
#define __BLUMIA_VPSUSAGE_HPP__

using namespace Glib;

class VPSState {
	
public:
	//variable
	gint64 bandwidthTotalByte;
	gint64 bandwidthUsedByte;
	gint64 nextClearTimeUNIX;
	//functions
	gdouble getLeftProportion();
	gdouble getBandwidthUsedGiB();
	gdouble getBandwidthLeftGiB();
	gint getBandwidthTotalGiB();
	gint getTimeLeftDay();
	gdouble getTimeLeftProportion();
	ustring getClearTimeStr();
};

gdouble VPSState::getLeftProportion() {
	return (1 - (gdouble)bandwidthUsedByte / bandwidthTotalByte);
}

gdouble VPSState::getBandwidthUsedGiB() {
	return (gdouble)this->bandwidthUsedByte / 1024 / 1024 / 1024;
}

gdouble VPSState::getBandwidthLeftGiB() {
	return (this->bandwidthTotalByte - this->bandwidthUsedByte) / (gdouble)1024 / 1024 / 1024;
}

gint VPSState::getBandwidthTotalGiB() {
	return this->bandwidthTotalByte / 1024 / 1024 / 1024;
}

gint VPSState::getTimeLeftDay() {
	return ceil((this->nextClearTimeUNIX - g_get_real_time()/(gdouble)1000000)/3600/24);
}

gdouble VPSState::getTimeLeftProportion() {
	gdouble dayLeft = (this->nextClearTimeUNIX - g_get_real_time()/(gdouble)1000000)/3600/24;
	g_print("%lf",dayLeft);
	return dayLeft>31 ? 1.0 : dayLeft/31;
}

ustring VPSState::getClearTimeStr() {
	GDate date, *pdate;
	char buf[61];
	g_date_set_time_t(pdate,(time_t)this->nextClearTimeUNIX);
	g_date_strftime(buf,100,"Bandwidth will be reset at %Y-%m-%d", pdate);
	ustring ret = buf;
	return ret;
}

#endif

 
