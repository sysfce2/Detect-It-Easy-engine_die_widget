/* Copyright (c) 2019-2024 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "dialogdiescandirectory.h"

#include "ui_dialogdiescandirectory.h"

DialogDIEScanDirectory::DialogDIEScanDirectory(QWidget *pParent, const QString &sDirName, const QString &sDatabasePath, const QString &sDatabasePathCustom)
    : XShortcutsDialog(pParent), ui(new Ui::DialogDIEScanDirectory)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

    g_sDatabasePath = sDatabasePath;
    g_sDatabasePathCustom = sDatabasePathCustom;

    //    Qt::WindowTitleHint

    connect(this, SIGNAL(resultSignal(QString)), this, SLOT(appendResult(QString)));

    ui->checkBoxScanSubdirectories->setChecked(true);
    ui->checkBoxRecursiveScan->setChecked(true);

    if (sDirName != "") {
        ui->lineEditDirectoryName->setText(QDir().toNativeSeparators(sDirName));
    }
}

DialogDIEScanDirectory::~DialogDIEScanDirectory()
{
    delete ui;
}

void DialogDIEScanDirectory::on_pushButtonOpenDirectory_clicked()
{
    QString sInitDirectory = ui->lineEditDirectoryName->text();

    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory") + QString("..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        ui->lineEditDirectoryName->setText(QDir().toNativeSeparators(sDirectoryName));
    }
}

void DialogDIEScanDirectory::on_pushButtonScan_clicked()
{
    QString sDirectoryName = ui->lineEditDirectoryName->text().trimmed();

    scanDirectory(sDirectoryName);
}

void DialogDIEScanDirectory::scanDirectory(const QString &sDirectoryName)
{
    if (sDirectoryName != "") {
        ui->textBrowserResult->clear();

        // TODO
        DiE_Script::OPTIONS options = {};
        options.bIsRecursiveScan = ui->checkBoxRecursiveScan->isChecked();
        options.bIsDeepScan = ui->checkBoxDeepScan->isChecked();
        options.bIsHeuristicScan = ui->checkBoxHeuristicScan->isChecked();
        options.bIsVerbose = ui->checkBoxVerbose->isChecked();
        options.bAllTypesScan = ui->checkBoxAllTypesScan->isChecked();
        options.bSubdirectories = ui->checkBoxScanSubdirectories->isChecked();
        options.nBufferSize = 2 * 1024 * 1024;  // TODO
        // TODO Filter
        // |flags|x all|

        DiE_Script dieScript;

        dieScript.loadDatabase(g_sDatabasePath, true);
        dieScript.loadDatabase(g_sDatabasePathCustom, false);

        DialogDIEScanProcess ds(this, &dieScript);
        connect(&ds, SIGNAL(scanResult(DiE_Script::SCAN_RESULT)), this, SLOT(scanResult(DiE_Script::SCAN_RESULT)), Qt::DirectConnection);
        ds.setData(sDirectoryName, options);
        ds.exec();
    }
}

void DialogDIEScanDirectory::scanResult(DiE_Script::SCAN_RESULT scanResult)
{
    // TODO
    QString sResult = QString("%1 %2 %3").arg(QDir().toNativeSeparators(scanResult.sFileName), QString::number(scanResult.nScanTime), tr("msec"));
    sResult += "\r\n";

    ScanItemModel model(&(scanResult.listRecords), 1, false);

    sResult += model.toFormattedString();

    emit resultSignal(sResult);
}

void DialogDIEScanDirectory::appendResult(const QString &sResult)
{
    ui->textBrowserResult->append(sResult);
}

void DialogDIEScanDirectory::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogDIEScanDirectory::on_pushButtonClear_clicked()
{
    ui->textBrowserResult->clear();
}

void DialogDIEScanDirectory::on_pushButtonSave_clicked()
{
    QString sFilter = QString("%1 (*.txt)").arg(tr("Text documents"));
    QString sSaveFileName = ui->lineEditDirectoryName->text() + QDir::separator() + "result";
    QString sFileName = QFileDialog::getSaveFileName(this, tr("Save result"), sSaveFileName, sFilter);

    if (!sFileName.isEmpty()) {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadWrite)) {
            QString sText = ui->textBrowserResult->toPlainText();
            file.write(sText.toUtf8().data());
            file.close();
        }
    }
}
