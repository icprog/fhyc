TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    basedb.cpp \
    inifile.cpp \
    LoadDay.cpp \
    LoadForNoTem.cpp \
    MyDate.cpp \
    my_occi.cpp \
    for_my_occi.cpp

OTHER_FILES += \
    db.ini

HEADERS += \
    basedb.h \
    inifile.h \
    LoadDay.h \
    LoadForNoTem.h \
    MyDate.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/occi.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/occiAQ.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/occiCommon.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/occiControl.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/occiData.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/occiObjects.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/xa.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ldap.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/nzerror.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/oci.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/oci1.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/oci8dp.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ociap.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ociapr.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ocidef.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ocidem.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ocidfn.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ociextp.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ocikpr.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ocixmldb.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ocixstream.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/odci.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/oratypes.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ori.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/orid.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/orl.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/oro.h \
    ../../../../../usr/local/oracle_client_12_1/sdk/include/ort.h \
    my_occi.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/oracle_client_12_1/lib/release/ -locci -lclntsh
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/oracle_client_12_1/lib/debug/ -locci -lclntsh
else:unix: LIBS += -L$$PWD/../../../../../usr/local/oracle_client_12_1/lib/ -locci -lclntsh

INCLUDEPATH += $$PWD/../../../../../usr/local/oracle_client_12_1
DEPENDPATH += $$PWD/../../../../../usr/local/oracle_client_12_1
INCLUDEPATH += $$PWD/../../../../../usr/local/oracle_client_12_1/sdk/include
