/*
 * libusp_bpm21.cpp
 *
 *  Created on: 2020��5��22��
 *      Author: JK
 */
#include "libusp_bpm21.h"
#include "cjson/cJSON_Utils.h"

int PRE_CDECL dll_test(int aa)
{
	cJSON *json = cJSON_Parse("");
	return (aa + 1);
}
