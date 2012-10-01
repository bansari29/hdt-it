/*
 * File: StopWatch.hpp
 * Last modified: $Date: 2011-08-21 05:35:30 +0100 (dom, 21 ago 2011) $
 * Revision: $Revision: 180 $
 * Last modified by: $Author: mario.arias $
 *
 * Copyright (C) 2012, Mario Arias
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 * Contacting the author:
 *   Mario Arias:               mario.arias@gmail.com
 *
 */

#include <sys/stat.h>
#include "fileUtil.hpp"

using namespace std;

uint64_t fileUtil::getSize(std::istream &in) {
	long long begin = in.tellg();
	in.seekg(0, std::ios::end);
	long long end = in.tellg();
	in.seekg(begin, std::ios::beg);

	return end-begin;
}

uint64_t fileUtil::getSize(const char *file) {
#ifdef WIN32
    ifstream in(file);
    in.seekg(0, std::ios::end);
    uint64_t size = in.tellg();
    in.close();
    return size;
#else
	struct stat fileStat;
	if(stat(file, &fileStat)==0) {
		return fileStat.st_size;
	}
	return 0;
#endif
}