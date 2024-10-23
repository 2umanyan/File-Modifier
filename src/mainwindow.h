#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QPushButton>
#include <QTimer>
#include "filemodifier.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void selectInputPath();
    void selectOutputPath();
    void startProcessing();
    void stopProcessing();
    void onModeChanged();

private:
    bool validateInputs();
    void setupLayout();

    QLineEdit *fileMaskEdit;
    QCheckBox *deleteInputFilesCheck;
    QLineEdit *inputPathEdit;
    QPushButton *inputPathButton;
    QLineEdit *outputPathEdit;
    QPushButton *outputPathButton;
    QCheckBox *overwriteCheck;
    QRadioButton *oneTimeRunRadioButton;
    QRadioButton *periodicRunRadioButton;
    QSpinBox *intervalSpinBox;
    QLineEdit *binaryValueEdit;
    QPushButton *startButton;
    QPushButton *stopButton;
    QTimer *timer;

    FileModifier fileModifier;
};

#endif
