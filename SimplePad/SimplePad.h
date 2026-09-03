#pragma once

#include <QtWidgets>
#include<QTranslator>
#include<QPrinter>
#include<QPrintDialog>
#include "ui_SimplePad.h"

class SimplePad : public QMainWindow
{
    Q_OBJECT

public:
    SimplePad(QWidget *parent = Q_NULLPTR);
    Ui::SimplePadClass& getUi();

private:
    bool maybeSave();
    bool loadDocument(const QString &filePath);
    bool saveDocument();
    bool saveDocumentAs();
    bool saveDocumentTo(const QString &filePath);
    void setCurrentFile(const QString &filePath);
    void updateWindowTitle();

    Ui::SimplePadClass ui;
    QString currentFilePath_;

    QTranslator translator;
    QStyle *style = nullptr;

    std::unique_ptr<QTreeView> treeView;
    std::unique_ptr<QFileSystemModel> model;

private slots:
    void info();
    void enLanguage();
    void ruLanguage();
    void darkTheme();
    void lightTheme();
    void openFolder();
    void selectItem(const QModelIndex&);
    void doPrint();
    void setFont();

public slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    
protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent* pe) override;
};
