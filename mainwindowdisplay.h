#pragma once
#include "os.h"

#include <QMainWindow>
#include <QStringList>

namespace Ui {
class MainWindowDisplay;
}

class MainWindowDisplay : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindowDisplay(QWidget *parent = nullptr);
    ~MainWindowDisplay();

signals:
    void sg_backToMainWindow();

private:
    Ui::MainWindowDisplay *ui;

    QStringList m_stringList;
};
