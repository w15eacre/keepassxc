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

#include "PreviewEntryAttachmentsDialog.h"
#include "core/EntryAttachments.h"
#include "ui_EntryAttachmentsDialog.h"

#include <QDialogButtonBox>
#include <QMimeDatabase>
#include <QTextCursor>
#include <QtGlobal>

PreviewEntryAttachmentsDialog::PreviewEntryAttachmentsDialog(QPointer<EntryAttachments> attachments, QWidget* parent)
    : QDialog(parent)
    , m_attachments(std::move(attachments))
    , m_ui(new Ui::EntryAttachmentsDialog)
{
    Q_ASSERT(m_attachments);

    m_ui->setupUi(this);

    setWindowTitle(tr("Preview entry attachment"));

    m_ui->titleEdit->setReadOnly(true);
    m_ui->attachmentTextEdit->setReadOnly(true);
    m_ui->errorLabel->setVisible(false);

    m_ui->dialogButtons->clear();
    m_ui->dialogButtons->addButton(QDialogButtonBox::Close);

    connect(m_ui->dialogButtons, SIGNAL(rejected()), this, SLOT(reject()));
}

PreviewEntryAttachmentsDialog::~PreviewEntryAttachmentsDialog() = default;

void PreviewEntryAttachmentsDialog::setAttachment(const QString& name)
{
    m_name = name;
    m_type = attachmentType(m_name);

    m_ui->titleEdit->setText(m_name);

    update();
}

void PreviewEntryAttachmentsDialog::update()
{
    if (m_type == AttachmentType::Unknown) {
        updateTextAttachment(tr("No preview available").toUtf8());
    } else if (const auto data = m_attachments->value(m_name); m_type == AttachmentType::Image) {
        updateImageAttachment(data);
    } else if (m_type == AttachmentType::PlantText) {
        updateTextAttachment(data);
    }
}

void PreviewEntryAttachmentsDialog::updateTextAttachment(const QByteArray& data)
{
    m_ui->attachmentTextEdit->setPlainText(QString::fromUtf8(data));
}

void PreviewEntryAttachmentsDialog::updateImageAttachment(const QByteArray& data)
{
    QImage image{};
    image.loadFromData(data);

    m_ui->attachmentTextEdit->clear();
    auto cursor = m_ui->attachmentTextEdit->textCursor();

    cursor.insertImage(image.scaled(m_ui->attachmentTextEdit->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

PreviewEntryAttachmentsDialog::AttachmentType PreviewEntryAttachmentsDialog::attachmentType(const QString& name) const
{
    const auto data = m_attachments->value(name);

    QMimeDatabase mimeDb{};
    const auto mime = mimeDb.mimeTypeForData(data);

    if (auto mimeName = mime.name(); mimeName.startsWith("image/")) {
        return AttachmentType::Image;
    } else if (mimeName.startsWith("text/")) {
        return AttachmentType::PlantText;
    }

    return AttachmentType::Unknown;
}

void PreviewEntryAttachmentsDialog::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);

    if (m_type == AttachmentType::Image) {
        update();
    }
}
