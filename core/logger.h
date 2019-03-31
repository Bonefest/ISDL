#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>
#include <string>

//Класс,который предоставляет интерфейс для простого логирования
class Logger {
private:
	std::ofstream outFile; 	//Файл вывода

	std::string buffer;		//Буфер,который хранит временную строку
							//Обеспечивает вывод в файл только при достижении некоторого лимита

	std::string outFileName;

	unsigned int bufSize;

	bool loaded;			//Хранит состояние загрузки файла вывода

	bool silent;			//Определяет тихий режим,при котором все неудачи при попытке в
							//обращении к логеру не будут оповещаться

public:
	explicit Logger(const char* fileName = "default_log.log",unsigned int bufferSize = 1024,bool silent=false);
	~Logger();

	//Добавляет строку сообщения в конец файла
	void log(std::string message);

	//Сбрасывает буфер в файл
	void flush();
};

#endif