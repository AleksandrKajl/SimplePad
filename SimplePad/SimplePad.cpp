#include "SimplePad.h"
#include<QDir>
#include<QFileInfo>
#include<QTextStream>
#include"FileSys.h"
#include"HotKeys.h"

#pragma warning(disable : 4834)

SimplePad::SimplePad(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    treeView = std::make_unique<QTreeView>();
    model = std::make_unique<QFileSystemModel>();
    treeView->setModel(model.get());
    
    ui.gridLayout_2->addWidget(treeView.get());
    treeView->hide();

    ui.mainToolBar->addAction(QIcon(":/Resource/Newfile.png"), tr("New"), this, SLOT(newFile()));
    ui.mainToolBar->addAction(QIcon(":/Resource/open-file.png"), tr("Open"), this, SLOT(openFile()));
    ui.mainToolBar->addAction(QIcon(":/Resource/save.png"), tr("Save"), this, SLOT(saveFile()));
    ui.mainToolBar->addAction(QIcon(":/Resource/print.png"), tr("Print"), this, SLOT(doPrint()));
    ui.mainToolBar->addAction(QIcon(":/Resource/font.png"), tr("Font edit"), this, SLOT(setFont()));
    ui.mainToolBar->addAction(QIcon(":/Resource/left.png"), tr("Alignment left"), this, [&] {ui.textEdit->setAlignment(Qt::AlignLeft); });
    ui.mainToolBar->addAction(QIcon(":/Resource/centr.png"), tr("Alignment center"), this, [&] {ui.textEdit->setAlignment(Qt::AlignCenter); });
    ui.mainToolBar->addAction(QIcon(":/Resource/right.png"), tr("Alignment center"), this, [&] {ui.textEdit->setAlignment(Qt::AlignRight); });


    connect(ui.action_New, SIGNAL(triggered()), SLOT(newFile()));
    connect(ui.action_Open_File, SIGNAL(triggered()), SLOT(openFile()));
    connect(ui.action_Save, SIGNAL(triggered()), SLOT(saveFile()));
    connect(ui.action_Save_As, SIGNAL(triggered()), SLOT(saveFileAs()));
    connect(ui.actionEnglish, SIGNAL(triggered()), SLOT(enLanguage()));
    connect(ui.action_Russian, SIGNAL(triggered()), SLOT(ruLanguage()));
    connect(ui.action_About_SimplePad, SIGNAL(triggered()), SLOT(info()));
    connect(ui.action_Dark, SIGNAL(triggered()), SLOT(darkTheme()));
    connect(ui.action_Light, SIGNAL(triggered()), SLOT(lightTheme()));
    connect(ui.actionOpe_n_folder_as_project, SIGNAL(triggered()), SLOT(openFolder()));
    connect(treeView.get(), SIGNAL(doubleClicked(const QModelIndex &)), SLOT(selectItem(const QModelIndex &)));
    connect(ui.action_Print, SIGNAL(triggered()), SLOT(doPrint()));
    connect(ui.textEdit->document(), &QTextDocument::modificationChanged,
        this, [this] { updateWindowTitle(); });

//Установка русской локализации по умолчанию
    ruLanguage();
    lightTheme();
    setCurrentFile(QString());

}

void SimplePad::newFile()
{
    if (!maybeSave())
        return;

    ui.textEdit->clear();
    setCurrentFile(QString());
    ui.textEdit->document()->setModified(false);
}

void SimplePad::saveFile()
{
    saveDocument();
}

void SimplePad::saveFileAs()
{
    saveDocumentAs();
}

void SimplePad::openFile()
{
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"),
        QDir::currentPath(), tr("Text file (*.txt);;All files (*.*)"));
    if (filePath.isEmpty())
        return;

    if (!maybeSave())
        return;

    loadDocument(filePath);
}

bool SimplePad::maybeSave()
{
    if (!ui.textEdit->document()->isModified())
        return true;

    const QMessageBox::StandardButton answer = QMessageBox::warning(this, tr("SimplePad"),
        tr("The document has been modified. Do you want to save your changes?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (answer == QMessageBox::Save)
        return saveDocument();

    return answer == QMessageBox::Discard;
}

bool SimplePad::loadDocument(const QString &filePath)
{
    QString text;
    QString errorMessage;
    if (!FileSys::readTextFile(filePath, text, errorMessage))
    {
        QMessageBox::warning(this, tr("Open file"), tr("Could not open the file: %1").arg(errorMessage));
        return false;
    }

    ui.textEdit->setPlainText(text);
    setCurrentFile(filePath);
    ui.textEdit->document()->setModified(false);
    return true;
}

bool SimplePad::saveDocument()
{
    if (currentFilePath_.isEmpty())
        return saveDocumentAs();

    return saveDocumentTo(currentFilePath_);
}

bool SimplePad::saveDocumentAs()
{
    const QString initialPath = currentFilePath_.isEmpty() ? QDir::currentPath() : currentFilePath_;
    const QString filePath = QFileDialog::getSaveFileName(this, tr("Save file"), initialPath,
        tr("Text file (*.txt);;All files (*.*)"));
    if (filePath.isEmpty())
        return false;

    return saveDocumentTo(filePath);
}

bool SimplePad::saveDocumentTo(const QString &filePath)
{
    QString errorMessage;
    if (!FileSys::writeTextFile(filePath, ui.textEdit->toPlainText(), errorMessage))
    {
        QMessageBox::warning(this, tr("Save file"), tr("Could not save the file: %1").arg(errorMessage));
        return false;
    }

    setCurrentFile(filePath);
    ui.textEdit->document()->setModified(false);
    return true;
}

void SimplePad::setCurrentFile(const QString &filePath)
{
    currentFilePath_ = filePath;
    updateWindowTitle();
}

void SimplePad::updateWindowTitle()
{
    QString fileName = currentFilePath_.isEmpty() ? tr("Untitled") : QFileInfo(currentFilePath_).fileName();
    if (ui.textEdit->document()->isModified())
        fileName += "*";

    setWindowTitle(tr("%1 - SimplePad").arg(fileName));
}

void SimplePad::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}

void SimplePad::info()
{
    QString text;
    QString errorMessage;
    if (!FileSys::readTextFile(":/Resource/info.txt", text, errorMessage))
    {
        QMessageBox::warning(this, tr("Info"), tr("Could not load information: %1").arg(errorMessage));
        return;
    }

    QMessageBox::information(this, tr("Info"), text);

}

void SimplePad::keyPressEvent(QKeyEvent* pe)
{
    HotKeys hK;

    if(hK.keysProcessing(this, pe))
        QWidget::keyPressEvent(pe);

}

void SimplePad::setFont()
{
    ui.textEdit->setFont(QFontDialog::getFont(0, ui.textEdit->font()));

}


void SimplePad::doPrint()
{
    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    if (dlg.exec() == QDialog::Accepted)
        ui.textEdit->print(&printer);
}

void SimplePad::ruLanguage()
{
    translator.load("./simplepad_ru");
    qApp->installTranslator(&translator);
    ui.retranslateUi(this);
    updateWindowTitle();
}

void SimplePad::enLanguage()
{
    translator.load("./simplepad_en");
    qApp->installTranslator(&translator);
    ui.retranslateUi(this);
    updateWindowTitle();
}

void SimplePad::lightTheme()
{
    this->setStyleSheet(nullptr);
}

void SimplePad::darkTheme()
{
    QString styleSheet;
    QString errorMessage;
    if (!FileSys::readTextFile(":/Resource/dark.qss", styleSheet, errorMessage))
    {
        QMessageBox::warning(this, tr("Dark theme"), tr("Could not load the theme: %1").arg(errorMessage));
        return;
    }

    setStyleSheet(styleSheet);
}

Ui::SimplePadClass& SimplePad::getUi()
{
    return ui;
}

void SimplePad::openFolder()
{
    QString str = QFileDialog::getExistingDirectory(this, tr("Select folder"), "", QFileDialog::ShowDirsOnly);
    model->setRootPath(QDir::currentPath());
    treeView->setRootIndex(model->index(str));

    for (int i = 1; i < model->columnCount(); ++i)
        treeView->hideColumn(i);

    treeView->setHeaderHidden(true);
    treeView->show();

}

void SimplePad::selectItem(const QModelIndex &index)
{
    const QString filePath = model->filePath(index);
    if (!maybeSave())
        return;

    loadDocument(filePath);
}
