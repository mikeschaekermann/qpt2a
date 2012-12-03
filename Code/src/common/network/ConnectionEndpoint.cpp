#include "ConnectionEndpoint.h"

ConnectionEndpoint::ConnectionEndpoint(boost::asio::ip::udp::endpoint endpoint) : m_endpoint(endpoint), m_uiLocalPacketId(0), m_uiRemotePacketId(0)
{

}