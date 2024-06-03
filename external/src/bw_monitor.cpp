#include "BW_monitor.h"

Monitor_window* BW_monitor::get_window(string id){
	if(window_map.find(id) == window_map.end()){
		window_map[id] = new Monitor_window();
	}
	return window_map[id];
}

uint32_t BW_monitor::get_window_data_size(string id){
	Monitor_window* current_window = get_window(id);
	return current_window->data_size;
}

cf_throughput BW_monitor::get_average_bw(string id){
	Monitor_window* current_window = get_window(id);
	cf_throughput bw = cf_throughput(current_window->TotalDataSize, current_window->TotalTime);
	return bw;
}

void BW_monitor::set_window_max_data_size(string id, uint32_t max_data_size){
	Monitor_window* current_window = get_window(id);
	current_window->max_data_size = max_data_size;
}

bool BW_monitor::can_send(string id, uint32_t package_size_byte){
	Monitor_window* current_window = get_window(id);
	cf_time current_time = cf_time_stamp();
	if(current_time > current_window->end){
		current_window->start = ((uint32_t)((current_time - current_window->end)/ current_window->window_size )) * current_window->window_size +current_window->end;
		current_window->end = current_window->start + current_window->window_size;
		current_window->data_size = 0;
		current_window->TotalTime += current_window->window_size;
	}else if(current_time < current_window->start){
		// do nothing
	}else{
		if( current_window->data_size + package_size_byte <= current_window->max_data_size){
			current_window->data_size += package_size_byte;
			current_window->TotalDataSize += cf_data_size(package_size_byte, CF_BYTE);
			return true;
		}
	}
	return false;
}


