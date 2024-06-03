
#ifndef INCLUDE_BW_MONITOR_H_
#define INCLUDE_BW_MONITOR_H_

#include <map>
#include <string>
#include "cofluent.h"

using namespace std;
using namespace sc_core;
using namespace cf_dt;
using namespace cf_pk;
using namespace cf_core;

class Monitor_window {
	public:
	    cf_data_size TotalDataSize = cf_data_size(0, CF_BYTE);
		cf_time TotalTime = cf_time(0, CF_NS);
		cf_time start;
		cf_time end;
		uint32_t data_size = 0;
		uint32_t max_data_size = 1024;
		cf_time window_size = cf_time(100, CF_NS);
};

class BW_monitor {
	public:
		Monitor_window* get_window(string id);
		uint32_t get_window_data_size(string id);
		cf_throughput get_average_bw(string id);
		void set_window_max_data_size(string id, uint32_t max_data_size);
		bool can_send(string id, uint32_t package_size_byte);
		BW_monitor(const BW_monitor&)=delete;
		BW_monitor& operator=(const BW_monitor&)=delete;
		static BW_monitor& get_instance(){
			static BW_monitor instance;
			return instance;
		}
	private:
		map<string, Monitor_window*> window_map;
		BW_monitor() {}
		~BW_monitor() {
			for (auto it = window_map.begin(); it != window_map.end(); it++) {
				delete it -> second;
			}
		}
};


#endif /* INCLUDE_BW_MONITOR_H_ */
