#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

class Form;
class MapSettings;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(MapSettings* _settings, QWidget* parent=0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    Form* m_pForm;
    MapSettings* settings;
};

#endif // MAINWINDOW_H
