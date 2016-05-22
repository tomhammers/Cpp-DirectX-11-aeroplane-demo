/* If WiiMoteLib C# program is running it will update the state of the wiimote and add it to WiiMoteData.txt
This Class will scrape the data from the text file so it can be used in our program*/
#include "WiiMoteInfo.h"

WiiMoteInfo::WiiMoteInfo() {
	
}

bool WiiMoteInfo::getButtonAPressed() {
	return buttonAPressed;
}

void WiiMoteInfo::setWiiMoteData() {
	fileStream.open("WiiMoteData.txt");
	outputFile.open("test.txt");

	while (std::getline(fileStream, line))
	{
		if (line.compare(0, 2, L"a ") == 0)  //"A button"
		{
			WCHAR first[5];
			WCHAR a[10];

			WCHAR oldStyleStr[200];
			wcscpy(oldStyleStr, line.c_str());
			// wiimotelib outputs 84 for true and 70 for false
			swscanf(oldStyleStr, L"%2s%s", first, &a);

			outputFile << "A: " << *a << std::endl;

			// 84 is ASCII for T -> True
			// Took me a while to realise what was going on!
			if (*a == 84) // ASCII for T -> True
			{
				buttonAPressed = true;
			}
			if (*a == 70) // and false as 70 (F)
			{
				buttonAPressed = false;
			}
		}

		if (line.compare(0, 2, L"r ") == 0)  //"roll "
		{
			WCHAR first[5];
			float r = 0.0f;

			WCHAR oldStyleStr[200];
			wcscpy(oldStyleStr, line.c_str());
			swscanf(oldStyleStr, L"%2s%f", first, &r);

			outputFile << "roll: " << r << std::endl;

			roll = r;

		}

		if (line.compare(0, 2, L"p ") == 0)  //"pitch "
		{
			WCHAR first[5];
			float p = 0.0f;

			WCHAR oldStyleStr[200];
			wcscpy(oldStyleStr, line.c_str());
			swscanf(oldStyleStr, L"%2s%f", first, &p);

			outputFile << "pitch: " << p << std::endl;

			pitch = p;
		}
	}
	this->~WiiMoteInfo();
}

WiiMoteInfo::~WiiMoteInfo() {
	// destructor
}