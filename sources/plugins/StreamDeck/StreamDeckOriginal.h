﻿#pragma once
#include "DeviceManager.h"
#include "Configuration.h"

class CStreamDeckOriginal : public CDeviceManager
{
public:
	explicit CStreamDeckOriginal(CConfiguration& configuration, shared::event::CEventHandler& mainEventHandler, int evtKeyStateReceived);

	virtual ~CStreamDeckOriginal();

	void reset() override;

	void resetKeyStream() override;

	void setBrightness(int percent) override;

	void setKeyImage(std::string& content, int& keyIndex, std::string& customText) override;

	std::vector<bool> readKeyStates() override;

	int getKeyIndex(std::vector<bool>& states);
	template <typename T>
	std::pair<bool, int > findInVector(const std::vector<T>& vecOfElements, const T& element);
private:
	static const int ImageReportLength;
	static const int KeyCols;
	static const int KeyRows;
	static const int KeyCount;
	static const int DataToSendLength;
	static int convertKeyIdOrigin(int& keyIndex);
};

