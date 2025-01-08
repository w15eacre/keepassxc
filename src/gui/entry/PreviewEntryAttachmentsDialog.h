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

#ifndef PREVIEWENTRYATTACHMENTSWIDGET_H
#define PREVIEWENTRYATTACHMENTSWIDGET_H

#include <QDialog>
#include <QPointer>

namespace Ui
{
    class EntryAttachmentsDialog;
}

class QByteArray;
class EntryAttachments;

class PreviewEntryAttachmentsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PreviewEntryAttachmentsDialog(QPointer<EntryAttachments> attachments, QWidget* parent = nullptr);
    ~PreviewEntryAttachmentsDialog() override;

    void setAttachment(const QString& name);

private:
    enum class AttachmentType
    {
        Image,
        PlantText,
        Unknown
    };

    void resizeEvent(QResizeEvent* event) override;

    AttachmentType attachmentType(const QString& name) const;

    void update();
    void updateTextAttachment(const QByteArray& data);
    void updateImageAttachment(const QByteArray& data);

private:
    QPointer<EntryAttachments> m_attachments;

    QScopedPointer<Ui::EntryAttachmentsDialog> m_ui;

    QString m_name;
    AttachmentType m_type{AttachmentType::Unknown};
};

#endif // PREVIEWENTRYATTACHMENTSWIDGET_H
