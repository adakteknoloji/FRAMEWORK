#include <QtPlugin>
#include "QSearchEdit.h"
#include "QSearchEdit_Plugin.h"


QSearchEdit_Plugin::QSearchEdit_Plugin(QObject * parent) : QObject(parent)
{
        initialized = false;
}

void QSearchEdit_Plugin::initialize(QDesignerFormEditorInterface * )
{
    if (initialized) {
        return;
    }
    initialized = true;
}

bool QSearchEdit_Plugin::isInitialized() const
{
        return initialized;
}

QWidget * QSearchEdit_Plugin::createWidget(QWidget *parent)
{
        return new QSearchEdit(parent);
}

QString QSearchEdit_Plugin::name() const
{
        return "QSearchEdit";
}

QString QSearchEdit_Plugin::group() const
{
        return "Input Widgets";
}

QIcon QSearchEdit_Plugin::icon() const
{
        return QIcon();
}

QString QSearchEdit_Plugin::toolTip() const
{
        return "Arama";
}

QString QSearchEdit_Plugin::whatsThis() const
{
        return "";
}

bool QSearchEdit_Plugin::isContainer() const
{
        return false;
}

QString QSearchEdit_Plugin::domXml() const
{
     return "<widget class=\"QSearchEdit\" name=\"searchEdit\">\n"
            " <property name=\"QSearchEdit\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>100</width>\n"
            "   <height>100</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string>Arama widget. "
            "</string>\n"
            " </property>\n"
            "</widget>\n";

}

QString QSearchEdit_Plugin::includeFile() const
{
        return "QSearchEdit.h";
}


Q_EXPORT_PLUGIN2(QSearchEdit_Plugin, QSearchEdit_Plugin)


