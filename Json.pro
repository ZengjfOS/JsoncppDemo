#-------------------------------------------------
#
# Project created by QtCreator 2015-11-15T05:19:06
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Json
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    json/json_internalarray.inl \
    json/json_internalmap.inl \
    json/json_reader.cpp \
    json/json_value.cpp \
    json/json_valueiterator.inl \
    json/json_writer.cpp

DISTFILES += \
    json/sconscript

HEADERS += \
    json/autolink.h \
    json/config.h \
    json/features.h \
    json/forwards.h \
    json/json.h \
    json/json_batchallocator.h \
    json/reader.h \
    json/value.h \
    json/writer.h

RESOURCES += \
    resource.qrc
