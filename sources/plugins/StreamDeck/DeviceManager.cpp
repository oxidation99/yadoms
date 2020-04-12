#include "DeviceManager.h"
#include "DeviceManagerHelper.h"
#include <hidapi.h>
#include <future>
#include <thread>
#include <shared/Log.h>

const uint16_t CDeviceManager::StreamDeckVendorId = 0x0FD9;


CDeviceManager::CDeviceManager(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler,
                               int evtKeyStateReceived)
	: m_configuration(configuration),
	  m_mainEventHandler(mainEventHandler),
	  m_mainEvtKeyStateReceived(evtKeyStateReceived),
	  m_handle(nullptr)
{
}

CDeviceManager::~CDeviceManager()
{
	m_readKeyThread->interrupt();
	m_readKeyThread->join();
}


void CDeviceManager::open()
{
	m_handle = hid_open(CDeviceManagerHelper::getDeviceInformation(m_configuration)->vendorID,
	                    CDeviceManagerHelper::getDeviceInformation(m_configuration)->productID, nullptr);
}

void CDeviceManager::close()
{
	hid_close(m_handle);
	m_readKeyThread->interrupt();
	m_readKeyThread->join();
}

void CDeviceManager::runKeyStateThread()
{
	m_readKeyThread = boost::make_shared<boost::thread>(boost::bind(&CDeviceManager::readHandler, this));
	m_readKeyThread->detach();
}

void CDeviceManager::readHandler()
{
	try
	{
		while (true)
		{
			auto keyState = readKeyStates();

			if (keyState.first)
			{
				m_mainEventHandler.postEvent(m_mainEvtKeyStateReceived, keyState.second);
			}
		}
	}
	catch (boost::thread_interrupted&)
	{
		YADOMS_LOG(error) << "StreamDeck thread interrupted";
	}
	catch (const std::exception& e)
	{
		YADOMS_LOG(error) << "Error readHandler, " << e.what();
	}
	catch (...)
	{
		YADOMS_LOG(error) << "Error readHandler, unknown error";
	}
}
