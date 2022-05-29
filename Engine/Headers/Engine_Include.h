/*!
 * \file Engine_Include.h
 * \date 2016/07/12 11:22
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Engine_Include_h__
#define Engine_Include_h__

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <ctime>
#include <algorithm>
#include <functional>
#include <iostream>
#include <io.h>
#include <atlimage.h>

#include <MMSystem.h>
#include <unordered_map.hpp>


#pragma warning(disable : 4251)

#pragma comment(lib,"winmm.lib")

#include <crtdbg.h>

#ifdef	_DEBUG
#ifndef __TOOL__H
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
#endif

#include "Engine_enum.h"
#include "Engine_macro.h"
#include "Engine_functor.h"
#include "Engine_fuction.h"
#include "Engine_struct.h"
#include "Engine_value.h"

using namespace std;
using namespace boost;

#endif // Engine_Include_h__