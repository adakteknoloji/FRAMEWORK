#include <QtPlugin>
#include "QCommaEdit.h"
#include "QCommaEdit_Plugin.h"

#include "../UTILS/virgul_ekle.cpp"
#include "../UTILS/round.cpp"

QCommaEdit_Plugin::QCommaEdit_Plugin(QObject *parent) : QObject(parent)
{
        initialized = false;
}

void QCommaEdit_Plugin::initialize(QDesignerFormEditorInterface * )
{
    if (initialized) {
            return;
    }
        initialized = true;
}

bool QCommaEdit_Plugin::isInitialized() const
{
        return initialized;
}

QWidget *QCommaEdit_Plugin::createWidget(QWidget *parent)
{
        return new QCommaEdit(parent);
}

QString QCommaEdit_Plugin::name() const
{
        return "QCommaEdit";
}

QString QCommaEdit_Plugin::group() const
{
        return "Input Widgets";
}

QIcon QCommaEdit_Plugin::icon() const
{
        return QIcon();
}

QString QCommaEdit_Plugin::toolTip() const
{
        return "";
}

QString QCommaEdit_Plugin::whatsThis() const
{
        return "";
}

bool QCommaEdit_Plugin::isContainer() const
{
        return false;
}

QString QCommaEdit_Plugin::domXml() const
{
    return "<widget class=\"QCommaEdit\" name=\"commaEdit\"></widget>";
}

QString QCommaEdit_Plugin::includeFile() const
{
        return "QCommaEdit.h";
}

/* 5.8 MACRO GECISI 09.02.2017 BURAK*/
Q_PLUGIN_METADATA ("com.adak.framework.QCommaEdit_Plugin")
//Q_EXPORT_PLUGIN2(QCommaEdit_Plugin, QCommaEdit_Plugin)


