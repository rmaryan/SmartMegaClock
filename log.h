/*
 * log.h
 *
 *  Copyright (c) 2019 Maryan Rachynskyy
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef log_h
#define log_h

// comment this line out to deactivate logging
#define LOGGER_ON

#ifdef LOGGER_ON
  #define LOGGER_PRINT(x)    Serial.print (x)
  #define LOGGER_PRINTLN(x)  Serial.println (x)
#else
  #define LOGGER_PRINT(x)
  #define LOGGER_PRINTLN(x)
#endif

#endif
