#include "ApplicationPreferences.h"

namespace ApplicationDefaultValues
{
    const QString interpreterLocation = QString();
    const QString compilerLocation = QString();
    const bool createASGFiles = false;
    const bool createGraphVizFiles = false;
    const bool quietCompilerOutput = false;

    const QColor unconnectedRailsColor = QColor(255, 0, 0);
    const QColor connectedRailsColor = QColor(0, 0, 0);
    const QColor functionNamesColor = QColor(170, 170, 170);
    const QColor functionCallsColor = QColor(0, 255, 0);
    const QColor stringsColor = QColor(0, 0, 255);
    const QColor variablesColor = QColor(255, 255, 0);

    const QStringList recentFiles = QStringList();
    const bool showLineNumbers = false;
    const bool showWhiteSpaces = false;
}

namespace ApplicationPreferences
{
    QString interpreterLocation;
    QString compilerLocation;
    bool createASGFiles;
    bool createGraphVizFiles;
    bool quietCompilerOutput;

    QColor unconnectedRailsColor;
    QColor connectedRailsColor;
    QColor functionNamesColor;
    QColor functionCallsColor;
    QColor stringsColor;
    QColor variablesColor;

    QStringList recentFiles;
    bool showLineNumbers;
    bool showWhiteSpaces;
}
