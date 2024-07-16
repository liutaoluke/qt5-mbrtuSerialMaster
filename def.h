#pragma once

// #define QT_NO_DEBUG_OUTPUT

#ifdef QT_NO_DEBUG_OUTPUT
#define qDebug(...) ((void) 0)
#endif
