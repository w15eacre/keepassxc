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

#ifndef NEWENTRYATTACHMENTSWIDGET_H
#define NEWENTRYATTACHMENTSWIDGET_H

#include <QDialog>
#include <QPointer>

#include <optional>

namespace Ui
{
    class NewEntryAttachmentsDialog;
}

class QByteArray;
class EntryAttachments;

class NewEntryAttachmentsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewEntryAttachmentsDialog(QPointer<EntryAttachments> attachments, QWidget* parent = nullptr);
    ~NewEntryAttachmentsDialog() override;

private slots:
    void saveAttachment();
    void fileNameTextChanged(const QString& fileName);

private:
    std::optional<QString> ValidateFileName(const QString& fileName) const;

private:
    QPointer<EntryAttachments> m_attachments;

    QScopedPointer<Ui::NewEntryAttachmentsDialog> m_ui;
};

#endif // NEWENTRYATTACHMENTSWIDGET_H
