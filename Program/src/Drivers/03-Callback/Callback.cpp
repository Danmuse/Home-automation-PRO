/*/*!
 * @file Callback.cpp
 * @par Author & Doxygen Editor
 * 	Daniel Di Módica ~ <a href = "mailto: danifabriziodmodica@gmail.com">danifabriziodmodica@@gmail.com</a>
 * @date 05/07/2023 20:02:36
 */

#include "Callback.h"

std::list<Callback*> g_callback_list;

Callback::Callback(void (*function)(void)) :
m_callback{function} {
	if (m_callback != nullptr) g_callback_list.push_back(this);
}

#pragma GCC push_options
#pragma GCC optimize ("O1")

void Callback::callbackMethod(void) {
	if (m_callback) m_callback();
}

#pragma GCC pop_options