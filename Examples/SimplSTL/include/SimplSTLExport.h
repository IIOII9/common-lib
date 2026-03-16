#pragma once
#pragma once
#ifndef SIMPLSTL_API
#ifdef SIMPLSTL_EXPORTS  // 该宏需在DLL项目设置中定义
#define SIMPLSTL_API __declspec(dllexport)
#define SIMPLSTL_TEMPLATE __declspec(dllexport)
#else
#define SIMPLSTL_API __declspec(dllimport)
#define SIMPLSTL_TEMPLATE
#endif
#endif
