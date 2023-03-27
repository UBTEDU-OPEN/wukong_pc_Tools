include (../windows.pri)
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += qutils \
           smtp \
           utils \
           ubxactiondata \
           ubxhttpclient
ubxactiondata.depends = utils
ubxhttpclient.depends = utils
