#pragma once
#include <string>
#include "globalDefs.h"

namespace HttpSocketHelper{

    UBXSOCKET_EXPORTS void initializeWinSocket();

    UBXSOCKET_EXPORTS  bool connectSocket(int volatile& handle, const std::string& remote_host_name,
        int remote_port_number, int timeout_millisecs);

    UBXSOCKET_EXPORTS int readSocket(int handle, bool connected, char* dest_buffer, int max_read_length, bool block_util_all_arrived);

    UBXSOCKET_EXPORTS int writeSocket(int handle, const char* source_buffer, int max_write_length);

    UBXSOCKET_EXPORTS bool resetSocketOptions(int handle);

    UBXSOCKET_EXPORTS bool setSocketBlockingState(int handle, bool block);

    UBXSOCKET_EXPORTS int waitForReadiness(int handle, bool for_ready, int timeout_msecs);

    UBXSOCKET_EXPORTS unsigned long getIntAddress(const char* ip_address);

}