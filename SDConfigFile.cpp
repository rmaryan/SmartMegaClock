/*
 * SD card configuration file reading library
 *
 * Copyright (c) 2014 Bradford Needham
 * (@bneedhamia, https://www.needhamia.com )
 * Licensed under LGPL version 2.1
 * a version of which should have been supplied with this file.
 *
 * Updated by Maryan Rachynskyy.
 *
 */

#include "SDConfigFile.h"
#include <errno.h>

#include "log.h"

/*
 * Opens the given file on the SD card.
 * Returns true if successful, false if not.
 *
 * configFileName = the name of the configuration file on the SD card.
 *
 * NOTE: SD.begin() must be called before calling our begin().
 */
boolean SDConfigFile::begin(const char *configFileName, uint8_t maxLineLength) {
	if (_file) {
		LOGGER_PRINT("File is already open: ");
		LOGGER_PRINTLN(configFileName);
		return false;
	}

	_lineLength = 0;
	_lineSize = 0;
	_valueIdx = -1;
	_atEnd = true;

	/*
	 * Allocate a buffer for the current line.
	 */
	_lineSize = maxLineLength + 1;
	_line = (char *) malloc(_lineSize);
	if (_line == 0) {
		LOGGER_PRINTLN("Out of memory");
		_atEnd = true;
		return false;
	}

	/*
	 * To avoid stale references to configFileName
	 * we don't save it. To minimize memory use, we don't copy it.
	 */

	_file = SD.open(configFileName, FILE_READ);
	if (!_file) {
		LOGGER_PRINT("Could not open SD file: ");
		LOGGER_PRINTLN(configFileName);
		_atEnd = true;
		return false;
	}

	// Initialize our reader
	_atEnd = false;

	return true;
}

/*
 * Cleans up our SDCOnfigFile object.
 */
void SDConfigFile::end() {
	if (_file) {
		_file.close();
	}

	if (_line != 0) {
		free(_line);
		_line = 0;
	}
	_atEnd = true;
}

/*
 * Reads the next name=value setting from the file.
 * Returns true if the setting was successfully read,
 * false if an error occurred or end-of-file occurred.
 */
boolean SDConfigFile::readNextSetting() {
	int bint;

	if (_atEnd) {
		return false;  // already at end of file (or error).
	}

	_lineLength = 0;
	_valueIdx = -1;

	/*
	 * Assume beginning of line.
	 * Skip blank and comment lines
	 * until we read the first character of the key
	 * or get to the end of file.
	 */
	while (true) {
		bint = _file.read();
		if (bint < 0) {
			_atEnd = true;
			return false;
		}

		if ((char) bint == '#') {
			// Comment line.  Read until end of line or end of file.
			while (true) {
				bint = _file.read();
				if (bint < 0) {
					_atEnd = true;
					return false;
				}
				if ((char) bint == '\r' || (char) bint == '\n') {
					break;
				}
			}
			continue; // look for the next line.
		}

		// Ignore line ends and blank text
		if ((char) bint == '\r' || (char) bint == '\n'
				|| (char) bint == ' ' || (char) bint == '\t') {
			continue;
		}

		break; // bint contains the first character of the name
	}

	// Copy from this first character to the end of the line.

	while (bint >= 0 && (char) bint != '\r' && (char) bint != '\n') {
		if (_lineLength >= _lineSize - 1) { // -1 for a terminating null.
			_line[_lineLength] = '\0';
			LOGGER_PRINT("Line too long: ");
			LOGGER_PRINTLN(_line);
			_atEnd = true;
			return false;
		}

		if ((char) bint == '=') {
			// End of Name; the next character starts the value.
			_line[_lineLength++] = '\0';
			_valueIdx = _lineLength;

		} else {
			_line[_lineLength++] = (char) bint;
		}

		bint = _file.read();
	}

	if (bint < 0) {
		_atEnd = true;
		// Don't exit. This is a normal situation:
		// the last line doesn't end in newline.
	}
	_line[_lineLength] = '\0';

	/*
	 * Sanity checks of the line:
	 *   No =
	 *   No name
	 * It's OK to have a null value (nothing after the '=')
	 */
	if (_valueIdx < 0) {
		LOGGER_PRINT("Missing '=' in line: ");
		LOGGER_PRINTLN(_line);
		_atEnd = true;
		return false;
	}
	if (_valueIdx == 1) {
		LOGGER_PRINT("Missing Name in line: =");
		LOGGER_PRINTLN(_line[_valueIdx]);
		_atEnd = true;
		return false;
	}

	// Name starts at _line[0]; Value starts at _line[_valueIdx].
	return true;

}

/*
 * Returns true if the most-recently-read setting name
 * matches the given name, false otherwise.
 */
boolean SDConfigFile::nameIs(const char *name) {
	if (strcmp(name, _line) == 0) {
		return true;
	}
	return false;
}

/*
 * Returns the name part of the most-recently-read setting.
 * or null if an error occurred.
 * WARNING: calling this when an error has occurred can crash your sketch.
 */
const char *SDConfigFile::getName() {
	if (_lineLength <= 0 || _valueIdx <= 1) {
		return 0;
	}
	return &_line[0];
}

/*
 * Returns the value part of the most-recently-read setting,
 * or null if there was an error.
 * WARNING: calling this when an error has occurred can crash your sketch.
 */
const char *SDConfigFile::getValue() {
	if (_lineLength <= 0 || _valueIdx <= 1) {
		return 0;
	}
	return &_line[_valueIdx];
}

/*
 * Returns a persistent, dynamically-allocated copy of the value part
 * of the most-recently-read setting, or null if a failure occurred.
 * 
 * Unlike getValue(), the return value of this function
 * persists after readNextSetting() is called or end() is called.
 */
char *SDConfigFile::copyValue() {
	char *result = 0;
	int length;

	if (_lineLength <= 0 || _valueIdx <= 1) {
		return 0; // begin() wasn't called, or failed.
	}

	length = strlen(&_line[_valueIdx]);
	result = (char *) malloc(length + 1);
	if (result == 0) {
		return 0; // out of memory
	}

	strcpy(result, &_line[_valueIdx]);

	return result;
}

/*
 * Returns the value part of the most-recently-read setting
 * as an integer, or 0 if an error occurred.
 * Base is 10 for decimal representation, 16 for hexadecimal, etc.
 * If conversion failed, errno is set to the corresponding value
 * (we do not use expceptions since they are expensive)
 */
int SDConfigFile::getIntValue(char base) {
	const char *str = getValue();
	if (!str) {
		errno = EINVAL;
		return 0;
	}
	errno = 0;
	return strtol(str, NULL, base);
}

long SDConfigFile::getLongValue(char base) {
	const char *str = getValue();
	if (!str) {
		errno = EINVAL;
		return 0;
	}
	errno = 0;
	return strtol(str, NULL, base);
}

/*
 * Returns the value part of the most-recently-read setting
 * as a boolean.
 * The value "true" corresponds to true;
 * all other values correspond to false.
 */
boolean SDConfigFile::getBooleanValue() {
	if (strcmp("Y", getValue()) == 0) {
		return true;
	}
	return false;
}

boolean SDConfigFile::beginWrite(const char* configFileName, uint8_t maxLineLength) {
	if (_file) {
		LOGGER_PRINT("File is already open: ");
		LOGGER_PRINTLN(configFileName);
		return false;
	}

	_file = SD.open(configFileName, O_READ | O_WRITE | O_CREAT);
	if (!_file) {
		LOGGER_PRINT("Could not open SD file to write: ");
		LOGGER_PRINTLN(configFileName);
		_atEnd = true;
		return false;
	}

	_maxLineLength = maxLineLength;

	// make sure all readers will fail
	_atEnd = true;

	return true;
}

boolean SDConfigFile::writeSetting(const char* name, const char* value) {

	int8_t nameLength = strnlen(name, _maxLineLength);
	int8_t valueLength = strnlen(value, _maxLineLength);

	if(nameLength + valueLength +1 > _maxLineLength) {
		LOGGER_PRINT("Line too long: ");
		LOGGER_PRINT(name);
		LOGGER_PRINT("=");
		LOGGER_PRINTLN(value);
		return false;
	}

	if(!_file.write(name, nameLength)) {
		LOGGER_PRINTLN("File writing error");
		return false;
	}
	if(!_file.write('=')) {
		LOGGER_PRINTLN("File writing error");
		return false;
	}
	if(!_file.write(value, valueLength)) {
		LOGGER_PRINTLN("File writing error");
		return false;
	}
	if(!_file.write("\r\n")) {
		LOGGER_PRINTLN("File writing error");
		return false;
	}

	return true;
}
