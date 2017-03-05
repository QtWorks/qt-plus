
QT += core gui multimedia xml network serialport widgets positioning qml quickwidgets

CONFIG += warn_off

TEMPLATE = lib

DEFINES += QTPLUS_LIBRARY
# Uncomment the following to generate QML grammar with Bison

# PRE_TARGETDEPS += $$PWD/source/cpp/QMLTree/QMLGrammarParser.cpp
# qmlGrammarTarget.target = $$PWD/source/cpp/QMLTree/QMLGrammarParser.cpp
# qmlGrammarTarget.depends = $$PWD/source/bison/QML.y
# qmlGrammarTarget.commands = bison $$PWD/source/bison/QML.y -o $$PWD/source/cpp/QMLTree/QMLGrammarParser.cpp
# QMAKE_EXTRA_TARGETS += qmlGrammarTarget

HEADERS += \
    source/cpp/qtplus_global.h \
    source/cpp/CFactory.h \
    source/cpp/CSingleton.h \
    source/cpp/CDumpable.h \
    source/cpp/CXMLNodable.h \
    source/cpp/CXMLNode.h \
    source/cpp/QTree.h \
    source/cpp/CPIDController.h \
    source/cpp/Image/CLargeMatrix.h \
    source/cpp/Image/CImageHistogram.h \
    source/cpp/Image/CImageUtilities.h \
    source/cpp/CSoundSynth.h \
    source/cpp/CTDMADevice.h \
    source/cpp/CStreamFactory.h \
    source/cpp/CConnectedStream.h \
    source/cpp/CSocketStream.h \
    source/cpp/CSerialStream.h \
    source/cpp/Web/CMJPEGClient.h \
    source/cpp/Web/CMJPEGServer.h \
    source/cpp/Web/CWebComposer.h \
    source/cpp/Web/CWebContext.h \
    source/cpp/Web/CHTTPServer.h \
    source/cpp/Web/CDynamicHTTPServer.h \
    source/cpp/Web/WebControls/CWebButton.h \
    source/cpp/Web/WebControls/CWebControl.h \
    source/cpp/Web/WebControls/CWebDiv.h \
    source/cpp/Web/WebControls/CWebFactory.h \
    source/cpp/Web/WebControls/CWebLabel.h \
    source/cpp/Web/WebControls/CWebPage.h \
    source/cpp/Web/WebControls/CWebTextBox.h \
    source/cpp/Web/WebControls/CWebTextEdit.h \
    source/cpp/Web/WebControls/CWebFileInput.h \
    source/cpp/ISerializable.h \
    source/cpp/CInterpolator.h \
    source/cpp/GeoTools/coordcnv.h \
    source/cpp/GeoTools/geocent.h \
    source/cpp/GeoTools/mgrs.h \
    source/cpp/GeoTools/polarst.h \
    source/cpp/GeoTools/tranmerc.h \
    source/cpp/GeoTools/ups.h \
    source/cpp/GeoTools/utm.h \
    source/cpp/GeoTools/UtmMgrs.h \
    source/cpp/CGeoUtilities.h \
    source/cpp/QMLTree/QMLItem.h \
    source/cpp/QMLTree/QMLNameValue.h \
    source/cpp/QMLTree/QMLComplexItem.h \
    source/cpp/QMLTree/QMLIdentifier.h \
    source/cpp/QMLTree/QMLType.h \
    source/cpp/QMLTree/QMLFile.h \
    source/cpp/QMLTree/QMLImport.h \
    source/cpp/QMLTree/QMLPropertyDeclaration.h \
    source/cpp/QMLTree/QMLPropertyAssignment.h \
    source/cpp/QMLTree/QMLPropertyAlias.h \
    source/cpp/QMLTree/QMLFunction.h \
    source/cpp/QMLTree/QMLFunctionCall.h \
    source/cpp/QMLTree/QMLQualifiedExpression.h \
    source/cpp/QMLTree/QMLBinaryOperation.h \
    source/cpp/QMLTree/QMLIf.h \
    source/cpp/QMLTree/QMLFor.h \
    source/cpp/QMLTree/QMLConditional.h \
    source/cpp/QMLTree/QMLTreeContext.h

SOURCES += \
    source/cpp/CDumpable.cpp \
    source/cpp/CXMLNodable.cpp \
    source/cpp/CXMLNode.cpp \
    source/cpp/CPIDController.cpp \
    source/cpp/Image/CLargeMatrix.cpp \
    source/cpp/Image/CImageHistogram.cpp \
    source/cpp/Image/CImageUtilities.cpp \
    source/cpp/CSoundSynth.cpp \
    source/cpp/CTDMADevice.cpp \
    source/cpp/CStreamFactory.cpp \
    source/cpp/CConnectedStream.cpp \
    source/cpp/CSocketStream.cpp \
    source/cpp/CSerialStream.cpp \
    source/cpp/Web/CMJPEGClient.cpp \
    source/cpp/Web/CMJPEGServer.cpp \
    source/cpp/Web/CWebComposer.cpp \
    source/cpp/Web/CWebContext.cpp \
    source/cpp/Web/CHTTPServer.cpp \
    source/cpp/Web/CDynamicHTTPServer.cpp \
    source/cpp/Web/WebControls/CWebButton.cpp \
    source/cpp/Web/WebControls/CWebControl.cpp \
    source/cpp/Web/WebControls/CWebDiv.cpp \
    source/cpp/Web/WebControls/CWebFactory.cpp \
    source/cpp/Web/WebControls/CWebLabel.cpp \
    source/cpp/Web/WebControls/CWebPage.cpp \
    source/cpp/Web/WebControls/CWebTextBox.cpp \
    source/cpp/Web/WebControls/CWebTextEdit.cpp \
    source/cpp/Web/WebControls/CWebFileInput.cpp \
    source/cpp/ISerializable.cpp \
    source/cpp/GeoTools/coordcnv.cpp \
    source/cpp/GeoTools/geocent.cpp \
    source/cpp/GeoTools/mgrs.cpp \
    source/cpp/GeoTools/polarst.cpp \
    source/cpp/GeoTools/tranmerc.cpp \
    source/cpp/GeoTools/ups.cpp \
    source/cpp/GeoTools/utm.cpp \
    source/cpp/GeoTools/UtmMgrs.cpp \
    source/cpp/CGeoUtilities.cpp \
    source/cpp/QMLTree/QMLItem.cpp \
    source/cpp/QMLTree/QMLNameValue.cpp \
    source/cpp/QMLTree/QMLComplexItem.cpp \
    source/cpp/QMLTree/QMLIdentifier.cpp \
    source/cpp/QMLTree/QMLType.cpp \
    source/cpp/QMLTree/QMLFile.cpp \
    source/cpp/QMLTree/QMLImport.cpp \
    source/cpp/QMLTree/QMLPropertyDeclaration.cpp \
    source/cpp/QMLTree/QMLPropertyAssignment.cpp \
    source/cpp/QMLTree/QMLPropertyAlias.cpp \
    source/cpp/QMLTree/QMLFunction.cpp \
    source/cpp/QMLTree/QMLFunctionCall.cpp \
    source/cpp/QMLTree/QMLQualifiedExpression.cpp \
    source/cpp/QMLTree/QMLBinaryOperation.cpp \
    source/cpp/QMLTree/QMLIf.cpp \
    source/cpp/QMLTree/QMLFor.cpp \
    source/cpp/QMLTree/QMLConditional.cpp \
    source/cpp/QMLTree/QMLTreeContext.cpp \
    source/cpp/QMLTree/QMLGrammarParser.cpp

DESTDIR = ../bin
MOC_DIR = ../moc/qt-plus
OBJECTS_DIR = ../obj/qt-plus

QMAKE_CLEAN *= $$DESTDIR/*$$TARGET*
QMAKE_CLEAN *= $$MOC_DIR/*$$TARGET*
QMAKE_CLEAN *= $$OBJECTS_DIR/*$$TARGET*

CONFIG(debug, debug|release) {
    TARGET = qt-plusd
}
CONFIG(release, debug|release) {
    TARGET = qt-plus
}

# Generate help
helpfile = qt-plus.qdocconf

QMAKE_POST_LINK += qdoc $$quote($$shell_path($$helpfile)) $$escape_expand(\\n\\t)

# Copy the index.html to the help directory
copyfile = source/misc/index.html
copydest = help/index.html

QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$shell_path($$copyfile)) $$quote($$shell_path($$copydest)) $$escape_expand(\\n\\t)
