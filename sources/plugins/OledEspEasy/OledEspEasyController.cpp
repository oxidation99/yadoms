#include "stdafx.h"
#include "OledEspEasyController.h"
#include <shared/Log.h>
#include <shared/http/HttpMethods.h>
#include "shared/StringExtension.h"

COledEspEasyController::COledEspEasyController(const COledEspEasyConfiguration& configuration)
	:m_configuration(configuration)
{
	
}

COledEspEasyController::~COledEspEasyController()
{
}

//nested enum with specific strings
DECLARE_ENUM_IMPLEMENTATION_NESTED(COledEspEasyController::ECommands, ECommands,
	((Oled)("oled"))
	((OledCommand)("oledcmd"))
);



void COledEspEasyController::switch_on()
{
	sendCommand(ECommands::kOledCommand, "on");
}

void COledEspEasyController::switch_off()
{
	sendCommand(ECommands::kOledCommand, "off");
}

void COledEspEasyController::clear_screen()
{
	sendCommand(ECommands::kOledCommand, "clear");
}


void COledEspEasyController::update_line(int iLine, int iCol, const std::string& text)
{
	sendCommand(ECommands::kOled, boost::lexical_cast<std::string>(iLine), boost::lexical_cast<std::string>(iCol), text);
}

void COledEspEasyController::sendCommand(const ECommands& command, const std::vector<std::string> & args) const
{
	try
	{
		std::ostringstream cmd;
		cmd << "http://" << m_configuration.getIPAddress();
		cmd << "/control?cmd=" << command.toString();
		for (auto&& arg : args)
		{
			cmd << "," << arg;
		}
		shared::CHttpMethods::sendGetRequest(cmd.str());
	}
	catch (std::exception &ex)
	{
		YADOMS_LOG(error) << "Fail to send command to ESPEasy : " << ex.what();
	}
	catch (...)
	{
		YADOMS_LOG(error) << "Fail to send command to ESPEasy : unknown error";
	}
}

