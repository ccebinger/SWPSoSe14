#ifndef APPLICATIONPREFERENCES_H
#define APPLICATIONPREFERENCES_H

#include <QString>
#include <QColor>
#include <QFileInfo>
#include <QFileInfoList>

enum SignType
{
    UNCONNECTED_OBJECT  = 1 << 0,
    CONNECTED_RAIL      = 1 << 1,
    FUNCTION_NAME       = 1 << 2,
    FUNCTION_CALL       = 1 << 3,
    SYSTEM_FUNCTION     = 1 << 4,
    STRING              = 1 << 5,
    VARIABLE            = 1 << 6
};

enum CurserMode
{
    NORMAL  = 1 << 0,
    SMART   = 1 << 1
};

namespace ApplicationDefaultValues
{
    extern const QString interpreterLocation;
    extern const QString compilerLocation;
    extern const bool createASGFiles;
    extern const bool createGraphVizFiles;
    extern const bool quietCompilerOutput;

    extern const QColor unconnectedRailsColor;
    extern const QColor connectedRailsColor;
    extern const QColor functionNamesColor;
    extern const QColor functionCallsColor;
    extern const QColor stringsColor;
    extern const QColor variablesColor;

    extern const QStringList recentFiles;
    extern const bool showLineNumbers;
    extern const bool showWhiteSpaces;
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
    extern QColor functionNamesColor;
    extern QColor functionCallsColor;
    extern QColor stringsColor;
    extern QColor variablesColor;

    extern QStringList recentFiles;
    extern bool showLineNumbers;
    extern bool showWhiteSpaces;
}

#endif // APPLICATIONPREFERENCES_H
