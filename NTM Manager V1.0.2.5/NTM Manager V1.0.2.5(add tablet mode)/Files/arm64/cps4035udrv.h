/*++

Module Name: 

    cps4035udrv.h

Abstract:

    This module contains the class definitions for the cps4035 user driver

Environment:

    user-mode

Revision History:

--*/

#ifndef CPS4035_USER_DEVICE_H
#define CPS4035_USER_DEVICE_H

#include <stdio.h>
#include <basetsd.h>
#include <minwindef.h>

#ifdef DLL_EXPORTS
#define CPS4035_API __declspec(dllexport)
#else
#define CPS4035_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tagCps4035EventType {
    EVT_TYPE_NULL         = 0,
    EVT_CPS4035_IDLE,
    EVT_CPS4035_RUN,
    EVT_CPS4035_UPDATING,
    EVT_PEN_CONNECTED,
    EVT_PEN_DISCONNECTED,
    EVT_PEN_BATT_CAP,
    EVT_HALL_STATE_CHG,
}  Cps4035EventType;

typedef enum tagWlsPenType {
    PEN_TYPE_NULL  = 0,
    PEN_NTM_TYPE_1 = 1,
    PEN_NTM_TYPE_2 = 2,
}  WlsPenType;

struct Cps4035Event {
    Cps4035EventType type;
    union {
        // EVT_PEN_CONNECTED
        WlsPenType    penType;

        // EVT_PEN_BATT_CAP
        UINT32        battCap;

        // EVT_CPS4035_UPDATING
        UINT32        progress;

        // EVT_HALL_STATE_CHG
        // 0 -- pen detached, 1 -- pen attached
        UINT32        hallState;
    };
};

typedef DWORD (*EVENT_CALLBACK_FUNC)(Cps4035Event* event, void* context);

typedef enum tagCps4035LogType {
    CPS4035LOG_TYPE_NULL,	///< Set to no log output
    CPS4035LOG_TYPE_CONSOLE,
    CPS4035LOG_TYPE_FILE,
    CPS4035LOG_TYPE_CUST_FUNC,
} Cps4035LogType;

typedef enum tagCps4035LogLevel{
    CPS4035_LOG_TRACE,
    CPS4035_LOG_DEBUG,
    CPS4035_LOG_INFO,
    CPS4035_LOG_WARN,
    CPS4035_LOG_ERROR,
    CPS4035_LOG_FATAL,
} Cps4035LogLevel;

typedef void (*LOG_CALLBACK_FUNC)(const char* str);

typedef struct tagCps4035LogInfo {
    Cps4035LogType  logType;
    INT32           logLevel;
    union {
        // for Cps4035LOG_TYPE_FILE
        FILE*             logFile;

        // for Cps4035LOG_TYPE_CUST_FUNC
        LOG_CALLBACK_FUNC logFunc;
    };

} Cps4035LogInfo;

typedef struct tagCps4035UserDevice Cps4035UserDevice;

extern "C" CPS4035_API
Cps4035UserDevice* getCps4035UserDevice();

//
// Befor exit program, call this function for free the cps4035 user device
//
extern "C" CPS4035_API
void putCps4035UserDevice(Cps4035UserDevice* userDevice);

//
// After get a getCps4035UserDevice object, it must call this function before
// call any other function.
//
extern "C" CPS4035_API
bool setCps4035LogInfo(Cps4035UserDevice* userDevice,
                       Cps4035LogInfo*    logInfo);

//
// After get a getCps4035UserDevice object, it must call this function before
// call any other function except setCps4035LogFunc.
//
extern "C" CPS4035_API
bool setCps4035EventCallback(Cps4035UserDevice* userDevice,
                             EVENT_CALLBACK_FUNC callback,
                             void* context);

//
// Must call this function before call start and updateFirmware function
// If the Cps4035UserDevice is in idle then call start and updateFirmware function.
//
extern "C" CPS4035_API
bool isCps4035Idle(Cps4035UserDevice* userDevice);

//
// Start cps4035 driver for normal mode
//
extern "C" CPS4035_API
bool startCps4035(Cps4035UserDevice* userDevice);

//
// Stop the cps4035 driver for release and close resources
//
extern "C" CPS4035_API
bool stopCps4035(Cps4035UserDevice* userDevice);

//
// Call this function for update firmware of cps4035
//
extern "C" CPS4035_API
bool updateCps4035Firmware(Cps4035UserDevice* userDevice,
                           const char* path);

#ifdef __cplusplus
}
#endif

#endif
