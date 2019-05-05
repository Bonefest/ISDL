#include "../logger.h"
#include <iostream>
#include <sstream>
#include <ctime>


using namespace MSDL;

Logger::Logger(const char* fileName,unsigned int bufferSize,bool _silent):bufSize(bufferSize),silent(_silent) {
	outFileName = fileName;
	
	outFile.open(outFileName.c_str(),std::ofstream::out | std::ofstream::app);

	if(!outFile.is_open()) {
		if(!silent) std::cerr << "Logger '" << fileName << "' can't be loaded.The logger system is inactive.";
		loaded = false;
	}

	loaded = true;
}

Logger::~Logger() {
	if(loaded) {
		outFile << buffer;
		outFile.close();
	}
}

void Logger::log(std::string message) {
	if(loaded) {
		time_t currentTime = time(NULL);
		tm* localTime = localtime(&currentTime);

		std::stringstream streamMessage;

		//Форматированый вывод в string-поток
		streamMessage << "[" <<
		((localTime->tm_hour) <= 9 ? "0":"") << localTime->tm_hour  << ":" << 
		((localTime->tm_min) <= 9 ? "0":"")  << localTime->tm_min   << "]" << 
		message << std::endl;

		//Если строка больше максимального размера буфера или строка не поместиться в буфер
		//из-за его переполнености
		if(streamMessage.str().length() > bufSize || streamMessage.str().length() + buffer.length() > bufSize) {
			outFile << buffer << streamMessage.str();
			buffer = "";
		} else 
			buffer += streamMessage.str();

	} else if(!silent) {
		std::cerr << "Logger can't log to '" << outFileName << "'!" << std::endl;
	}

}

void Logger::flush() {
	if(loaded) {
		outFile << buffer;
		buffer = "";
	}
}