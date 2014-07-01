#ifndef APPLICATIONPREFERENCES_H
#define APPLICATIONPREFERENCES_H

#include <QString>
#include <QColor>
#include <QFileInfo>
#include <QFileInfoList>


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
}

#endif // APPLICATIONPREFERENCES_H
