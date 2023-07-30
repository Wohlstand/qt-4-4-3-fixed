
FORMS       += forms/mainwindow.ui
QT +=       xmlpatterns webkit
SOURCES     += qobjectxmlmodel.cpp main.cpp mainwindow.cpp
HEADERS     += qobjectxmlmodel.h  mainwindow.h
RESOURCES   = queries.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/xmlpatterns/qobjectxmlmodel
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro *.xq *.html
sources.path = $$[QT_INSTALL_EXAMPLES]/xmlpatterns/qobjectxmlmodel
INSTALLS += target sources
