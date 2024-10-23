#include <QVBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setupLayout();

    connect(inputPathButton, &QPushButton::clicked, this, &MainWindow::selectInputPath);
    connect(outputPathButton, &QPushButton::clicked, this, &MainWindow::selectOutputPath);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startProcessing);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopProcessing);
    connect(oneTimeRunRadioButton, &QRadioButton::toggled, this, &MainWindow::onModeChanged);
    connect(periodicRunRadioButton, &QRadioButton::toggled, this, &MainWindow::onModeChanged);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, &fileModifier, &FileModifier::processFiles);
}

void MainWindow::setupLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    fileMaskEdit = new QLineEdit(this);
    deleteInputFilesCheck = new QCheckBox("Удалять входные файлы", this);
    inputPathEdit = new QLineEdit(this);
    inputPathButton = new QPushButton("Выбрать путь входных файлов", this);
    outputPathEdit = new QLineEdit(this);
    outputPathButton = new QPushButton("Выбрать путь выходных файлов", this);
    overwriteCheck = new QCheckBox("Перезаписывать существующие файлы", this);
    oneTimeRunRadioButton = new QRadioButton("Одноразовый запуск", this);
    periodicRunRadioButton = new QRadioButton("Периодический запуск", this);
    intervalSpinBox = new QSpinBox(this);
    intervalSpinBox->setRange(1, 3600);
    intervalSpinBox->setEnabled(false);
    binaryValueEdit = new QLineEdit(this);
    binaryValueEdit->setMaxLength(16);

    formLayout->addRow("Маска входных файлов", fileMaskEdit);
    formLayout->addRow(deleteInputFilesCheck);
    formLayout->addRow("Путь для входных файлов", inputPathEdit);
    formLayout->addWidget(inputPathButton);
    formLayout->addRow("Путь для выходных файлов", outputPathEdit);
    formLayout->addWidget(outputPathButton);
    formLayout->addRow(overwriteCheck);
    formLayout->addRow(oneTimeRunRadioButton);
    formLayout->addRow(periodicRunRadioButton);
    formLayout->addRow("Интервал опроса (с)", intervalSpinBox);
    formLayout->addRow("Значение для операции (8 байт в HEX)", binaryValueEdit);

    startButton = new QPushButton("Запустить", this);
    stopButton = new QPushButton("Остановить", this);
    stopButton->setEnabled(false);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(startButton);
    mainLayout->addWidget(stopButton);

    oneTimeRunRadioButton->setChecked(true);

    setFixedSize(sizeHint());
}

void MainWindow::selectInputPath()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Выберите папку для входных файлов");
    if (!dir.isEmpty()) {
        inputPathEdit->setText(dir);
    }
}

void MainWindow::selectOutputPath()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Выберите папку для выходных файлов");
    if (!dir.isEmpty()) {
        outputPathEdit->setText(dir);
    }
}

void MainWindow::startProcessing()
{
    if (validateInputs()) {
        fileModifier.setFileMask(fileMaskEdit->text());
        fileModifier.setDeleteInputFiles(deleteInputFilesCheck->isChecked());
        fileModifier.setInputPath(inputPathEdit->text());
        fileModifier.setOutputPath(outputPathEdit->text());
        fileModifier.setOverwriteFiles(overwriteCheck->isChecked());
        fileModifier.setBinaryValue(QByteArray::fromHex(binaryValueEdit->text().toUtf8()));

        if (oneTimeRunRadioButton->isChecked()) {
            fileModifier.processFiles();
            startButton->setEnabled(true);
            stopButton->setEnabled(false);
        } else {
            timer->start(intervalSpinBox->value() * 1000);
            startButton->setEnabled(false);
            stopButton->setEnabled(true);
        }
    }
}

void MainWindow::stopProcessing()
{
    timer->stop();
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}

void MainWindow::onModeChanged()
{
    intervalSpinBox->setEnabled(periodicRunRadioButton->isChecked());
}

bool MainWindow::validateInputs()
{
    if (fileMaskEdit->text().isEmpty() ||
        inputPathEdit->text().isEmpty() ||
        outputPathEdit->text().isEmpty() ||
        binaryValueEdit->text().length() != 16) {
        QMessageBox::warning(this, "Ошибка ввода", "Пожалуйста, заполните все поля правильно.");
        return false;
    }

    return true;
}
