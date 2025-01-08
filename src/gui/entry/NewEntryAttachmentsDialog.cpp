/*
 *  Copyright (C) 2021 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "NewEntryAttachmentsDialog.h"
#include "core/EntryAttachments.h"
#include "ui_EntryAttachmentsDialog.h"

#include <QMessageBox>
#include <QPushButton>

NewEntryAttachmentsDialog::NewEntryAttachmentsDialog(QPointer<EntryAttachments> attachments, QWidget* parent)
    : QDialog(parent)
    , m_attachments(std::move(attachments))
    , m_ui(new Ui::EntryAttachmentsDialog)
{
    Q_ASSERT(m_attachments);

    m_ui->setupUi(this);

    setWindowTitle(tr("New entry attachment"));

    m_ui->dialogButtons->clear();
    m_ui->dialogButtons->addButton(QDialogButtonBox::Ok);
    m_ui->dialogButtons->addButton(QDialogButtonBox::Cancel);

    connect(m_ui->dialogButtons, SIGNAL(accepted()), this, SLOT(saveAttachment()));
    connect(m_ui->dialogButtons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(m_ui->titleEdit, SIGNAL(textChanged(const QString&)), this, SLOT(fileNameTextChanged(const QString&)));

    fileNameTextChanged(m_ui->titleEdit->text());
}

NewEntryAttachmentsDialog::~NewEntryAttachmentsDialog() = default;

std::optional<QString> NewEntryAttachmentsDialog::ValidateFileName(const QString& fileName) const
{
    if (fileName.isEmpty()) {
        return tr("Attachment name cannot be empty");
    }

    if (m_attachments->hasKey(fileName)) {
        return tr("Attachment with the same name already exists");
    }

    return std::nullopt;
}

void NewEntryAttachmentsDialog::saveAttachment()
{
    auto fileName = m_ui->titleEdit->text();
    auto text = m_ui->attachmentTextEdit->toPlainText().toUtf8();

    if (auto error = ValidateFileName(fileName); error) {
        QMessageBox::warning(this, tr("Save attachment"), error.value());
        return;
    }

    m_attachments->set(fileName, text);

    accept();
}

void NewEntryAttachmentsDialog::fileNameTextChanged(const QString& fileName)
{
    const auto error = ValidateFileName(fileName);

    m_ui->errorLabel->setText(error.value_or(QString{}));
    m_ui->errorLabel->setVisible(error.has_value());

    if (auto okButton = m_ui->dialogButtons->button(QDialogButtonBox::Ok); okButton) {
        okButton->setDisabled(error.has_value());
    }
}
