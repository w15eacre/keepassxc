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
#include <QPushButton>
#include <QTextCursor>
#include <QtDebug>

PreviewEntryAttachmentsDialog::PreviewEntryAttachmentsDialog(QPointer<EntryAttachments> attachments, QWidget* parent)
    : QDialog(parent)
    , m_attachments(std::move(attachments))
    , m_ui(new Ui::EntryAttachmentsDialog)
{
    Q_ASSERT(m_attachments);

    m_ui->setupUi(this);

    setWindowTitle(tr("Preview entry attachment"));
    // Disable the help button in the title bar
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(m_attachments, &EntryAttachments::keyModified, [this](const QString& name) {
        if (m_name == name) {
            update();
        }
    });

    enableReadOnlyMode();
    initDialogButtons();
}

PreviewEntryAttachmentsDialog::~PreviewEntryAttachmentsDialog() = default;

void PreviewEntryAttachmentsDialog::initDialogButtons()
{
    m_ui->dialogButtons->clear();
    m_ui->dialogButtons->addButton(QDialogButtonBox::Close);
    m_ui->dialogButtons->addButton(QDialogButtonBox::Save);

    if (auto openButton = m_ui->dialogButtons->addButton(QDialogButtonBox::Open); openButton) {
        openButton->setText(tr("Open"));
    }

    connect(m_ui->dialogButtons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(m_ui->dialogButtons, &QDialogButtonBox::clicked, [this](QAbstractButton* button) {
        if (auto standartButton = m_ui->dialogButtons->standardButton(button);
            standartButton == QDialogButtonBox::Open) {
            emit openAttachment(m_name);
        } else if (standartButton == QDialogButtonBox::Save) {
            emit saveAttachment(m_name);
        }
    });
}

void PreviewEntryAttachmentsDialog::enableReadOnlyMode()
{
    m_ui->titleEdit->setReadOnly(true);
    m_ui->attachmentTextEdit->setReadOnly(true);
    m_ui->errorLabel->setVisible(false);
}

void PreviewEntryAttachmentsDialog::setAttachment(const QString& name)
{
    m_name = name;
    m_type = attachmentType(m_name);

    m_ui->titleEdit->setText(m_name);

    update();
}

void PreviewEntryAttachmentsDialog::update()
{
    if (m_type == core::MimeType::Unknown) {
        updateTextAttachment(tr("No preview available").toUtf8());
    } else if (const auto data = m_attachments->value(m_name); m_type == core::MimeType::Image) {
        updateImageAttachment(data);
    } else if (m_type == core::MimeType::PlantText) {
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

core::MimeType PreviewEntryAttachmentsDialog::attachmentType(const QString& name) const
{
    const auto data = m_attachments->value(name);

    QMimeDatabase mimeDb{};
    const auto mime = mimeDb.mimeTypeForData(data);

    return core::toMimeType(mime.name());
}

void PreviewEntryAttachmentsDialog::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);

    if (m_type == core::MimeType::Image) {
        update();
    }
}
