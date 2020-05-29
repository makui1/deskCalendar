#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H
#include <QDialog>

class QPushButton;
class QLabel;
class QRadioButton;
class QGroupBox;
class QCheckBox;
class QFrame;
class QLineEdit;
class QVBoxLayout;
class QHBoxLayout;
class QJsonDocument;

struct Settings{
    QColor themeColor;
    QColor taskColor;
    bool isPopUP;
    QString prefix;
    bool enableSuffix;
};

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog() {delete mainLayout;};
    static Settings getSettings();


private:
    QLabel *labelThemeColor;
    QPushButton *themeColor;
    QLabel *labelTaskColor;
    QPushButton *defaultTaskColor;
    QGroupBox *groupBox;
    QRadioButton *sysTray;
    QRadioButton *popUP;
    QCheckBox *prefix;
    QCheckBox *suffix;
    QLineEdit *prefixEdit;
    QFrame *splitter1;
    QFrame *splitter2;

    QVBoxLayout *mainLayout;
    QHBoxLayout *prefixLayout;
    QVBoxLayout *groupBoxLayout;
    QHBoxLayout *themeCorlorLayout;
    QHBoxLayout *taskColorLayout;

    static QJsonDocument *document;

    static void setDefault();
};

#endif // SETTINGDIALOG_H
