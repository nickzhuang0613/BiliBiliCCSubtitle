/*
 * Copyright 2020 NathanLi
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef BILIBILICCSUBTITLE_CCJSON_DOWNLOADER_H
#define BILIBILICCSUBTITLE_CCJSON_DOWNLOADER_H
#include <iostream>
#include <regex>
#include "curl_helper.h"
int do_download_json(std::string const & inputfile,int p_start,int p_end,bool auto_convert=false);
#endif //BILIBILICCSUBTITLE_CCJSON_DOWNLOADER_H
