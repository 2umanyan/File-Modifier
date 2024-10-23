#ifndef FILEMODIFIER_H
#define FILEMODIFIER_H

#include <QObject>
#include <QString>

class FileModifier : public QObject {
    Q_OBJECT

public:
    explicit FileModifier(QObject *parent = nullptr);

    void setFileMask(const QString &mask);
    void setDeleteInputFiles(bool deleteFiles);
    void setInputPath(const QString &path);
    void setOutputPath(const QString &path);
    void setOverwriteFiles(bool overwrite);
    void setBinaryValue(const QByteArray &value);

public slots:
    void processFiles();

private:
    QString fileMask;
    bool deleteInputFiles;
    QString inputPath;
    QString outputPath;
    bool overwriteFiles;
    QByteArray binaryValue;
};

#endif
