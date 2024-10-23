#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include "filemodifier.h"

FileModifier::FileModifier(QObject *parent)
    : QObject(parent), deleteInputFiles(false), overwriteFiles(false)
{
}

void FileModifier::setFileMask(const QString &mask)
{
    fileMask = mask;
}

void FileModifier::setDeleteInputFiles(bool deleteFiles)
{
    deleteInputFiles = deleteFiles;
}

void FileModifier::setInputPath(const QString &path)
{
    inputPath = path;
}

void FileModifier::setOutputPath(const QString &path)
{
    outputPath = path;
}

void FileModifier::setOverwriteFiles(bool overwrite)
{
    overwriteFiles = overwrite;
}

void FileModifier::setBinaryValue(const QByteArray &value)
{
    binaryValue = value;
}

void FileModifier::processFiles()
{
    QDir dir(inputPath);
    QStringList files = dir.entryList(QStringList() << fileMask, QDir::Files);

    foreach (QString fileName, files) {
        QString inputFilePath = dir.absoluteFilePath(fileName);
        QFile inputFile(inputFilePath);

        if (!inputFile.open(QIODevice::ReadOnly)) {
            continue;
        }

        QByteArray fileData = inputFile.readAll();
        inputFile.close();

        for (int i = 0; i < fileData.size(); ++i) {
            fileData[i] = fileData[i] ^ binaryValue[i % 8];
        }

        QString outputFilePath = outputPath + "/" + fileName;
        if (!overwriteFiles) {
            int counter = 1;
            while (QFile::exists(outputFilePath)) {
                outputFilePath = outputPath + "/" + QFileInfo(fileName).completeBaseName() + QString("_%1").arg(counter++) + "." + QFileInfo(fileName).suffix();
            }
        }

        QFile outputFile(outputFilePath);
        if (outputFile.open(QIODevice::WriteOnly)) {
            outputFile.write(fileData);
            outputFile.close();
        }

        if (deleteInputFiles) {
            inputFile.remove();
        }
    }
}
