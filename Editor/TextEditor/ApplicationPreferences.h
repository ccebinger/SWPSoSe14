#ifndef APPLICATIONPREFERENCES_H
#define APPLICATIONPREFERENCES_H

#include <QString>
#include <QColor>
#include <QFileInfo>
#include <QFileInfoList>

namespace ApplicationConstants
{

    enum Direction
    {
        SE  = 1 << 0,
        S   = 1 << 1,
        SW  = 1 << 2,
        E   = 1 << 3,
        W   = 1 << 4,
        NE  = 1 << 5,
        N   = 1 << 6,
        NW  = 1 << 7
    };

    enum SignType
    {
        UNCONNECTED_OBJECT  = 1 << 0,
        CONNECTED_RAIL      = 1 << 1,
        FUNCTION_NAME       = 1 << 2,
        FUNCTION_CALL       = 1 << 3,
        SYSTEM_FUNCTION     = 1 << 4,
        STRING              = 1 << 5,
        VARIABLE            = 1 << 6,
        GRAB                = 1 << 7
    };

    enum CursorMode
    {
        NORMAL  = 1 << 0,
        SMART   = 1 << 1
    };
}

namespace ApplicationDefaultValues
{
    extern const QString interpreterLocation;
    extern const QString compilerLocation;
    extern const bool createASGFiles;
    extern const bool createGraphVizFiles;
    extern const bool quietCompilerOutput;

    extern const QColor unconnectedRailsColor;
    extern const QColor connectedRailsColor;
    extern const QColor systemFunctionColor;
    extern const QColor functionNamesColor;
    extern const QColor functionCallsColor;
    extern const QColor stringsColor;
    extern const QColor variablesColor;
    extern const QColor grabColor;

    extern const QStringList recentFiles;
    extern const bool showLineNumbers;
    extern const bool showWhiteSpaces;
    extern const bool showEditorLines;
    extern const ApplicationConstants::CursorMode cursorMode;
}

namespace ApplicationPreferences
{
    extern QString interpreterLocation;
    extern QString compilerLocation;
    extern bool createASGFiles;
    extern bool createGraphVizFiles;
    extern bool quietCompilerOutput;

    extern QColor unconnectedRailsColor;
    extern QColor connectedRailsColor;
    extern QColor systemFunctionColor;
    extern QColor functionNamesColor;
    extern QColor functionCallsColor;
    extern QColor stringsColor;
    extern QColor variablesColor;
    extern QColor grabColor;

    extern QStringList recentFiles;
    extern bool showLineNumbers;
    extern bool showWhiteSpaces;
    extern bool showEditorLines;
    extern ApplicationConstants::CursorMode cursorMode;
}

#endif // APPLICATIONPREFERENCES_H
