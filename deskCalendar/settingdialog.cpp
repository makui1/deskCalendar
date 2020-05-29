#include "settingdialog.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDebug>

QJsonDocument *SettingDialog::document = nullptr;

SettingDialog::SettingDialog(QWidget *parent) : QDialog(parent),
    mainLayout(new QVBoxLayout),prefixLayout(new QHBoxLayout),groupBoxLayout(new QVBoxLayout),themeCorlorLayout(new QHBoxLayout),taskColorLayout(new QHBoxLayout),
    labelThemeColor(new QLabel),labelTaskColor(new QLabel),themeColor(new QPushButton),defaultTaskColor(new QPushButton),
    groupBox(new QGroupBox),sysTray(new QRadioButton),popUP(new QRadioButton),
    prefix(new QCheckBox),suffix(new QCheckBox),prefixEdit(new QLineEdit),
    splitter1(new QFrame),splitter2(new QFrame)
{
    labelTaskColor->setText("新任务默认颜色");
    labelThemeColor->setText("日历主题色");
    int r = 100,g = 100,b = 100;
    themeColor->setStyleSheet("background:rgb("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+")");
    defaultTaskColor->setStyleSheet("background:rgb("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+")");
    themeCorlorLayout->addWidget(labelThemeColor);
    themeCorlorLayout->addWidget(themeColor);
    taskColorLayout->addWidget(labelTaskColor);
    taskColorLayout->addWidget(defaultTaskColor);
    groupBox->setTitle("任务提醒方式");
    sysTray->setText("系统通知栏提醒");
    popUP->setText("弹出窗口提示");
    groupBoxLayout->addWidget(sysTray);
    groupBoxLayout->addWidget(popUP);
    sysTray->setChecked(true);
    groupBox->setLayout(groupBoxLayout);
    prefix->setText("为文件类任务自动添加前缀");
    prefixEdit->setText("处理文件");
    prefixEdit->setEnabled(false);
    suffix->setText("添加文件类任务时默认任务名包含文件名后缀");
    prefixLayout->addWidget(prefix);
    prefixLayout->addWidget(prefixEdit);
    splitter1->resize(splitter1->width(),1);
    splitter1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    splitter2->resize(splitter1->width(),1);
    splitter2->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    mainLayout->addLayout(themeCorlorLayout);
    mainLayout->addLayout(taskColorLayout);
    mainLayout->addWidget(splitter1);
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(splitter2);
    mainLayout->addLayout(prefixLayout);
    mainLayout->addWidget(suffix);

    setLayout(mainLayout);
    Settings settings = getSettings();
    qDebug() << settings.themeColor << settings.taskColor
             << settings.isPopUP << settings.prefix << settings.enableSuffix;
}

Settings SettingDialog::getSettings()
{
    if(!QFile::exists("settings.json")) setDefault();
    QFile settingFile("settings.json");
    if(!settingFile.open(QIODevice::ReadOnly)){
        qDebug() << "Could not open the file";
        return Settings();
    }
    QByteArray data = settingFile.readAll();
    if(!document) document = new QJsonDocument;
    document->fromJson(data);
    QJsonObject object = document->object();
    Settings settings;
    QJsonObject tempObject;
    if(object.contains("themeColor")) tempObject = object["themeColor"].toObject();
    int r,g,b;
    r = tempObject["r"].toInt();
    g = tempObject["g"].toInt();
    b = tempObject["b"].toInt();
    settings.themeColor = QColor(r,g,b);
    if(object.contains("taskColor")) tempObject = object["taskColor"].toObject();
    r = tempObject["r"].toInt();
    g = tempObject["g"].toInt();
    b = tempObject["b"].toInt();
    settings.taskColor = QColor(r,g,b);
    if(object.contains("isPopUP")) settings.isPopUP = object["isPopUp"].toBool();
    if(object.contains("prefix")) settings.prefix = object["prefix"].toString();
    if(object.contains("enableSuffix")) settings.enableSuffix = object["enableSuffix"].toBool();

    return settings;
}

void SettingDialog::setDefault()
{
    QFile settingFile("settings.json");
    if(!settingFile.open(QIODevice::WriteOnly)){
        qDebug() << "Could not open the file!";
        return;
    }
    QJsonObject object;
    QJsonObject themeColorObject;
    QJsonObject taskColorObject;
    themeColorObject["r"] = 100;
    themeColorObject["g"] = 100;
    themeColorObject["b"] = 100;
    object["themeColor"] = themeColorObject;
    taskColorObject = themeColorObject;
    object["taskColor"] = taskColorObject;
    object["isPopUP"] = false;
    object["prefix"] = QString();
    object["enableSuffix"] = false;
    QJsonDocument document(object);
    settingFile.write(document.toJson());
}
