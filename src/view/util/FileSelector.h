//
// Created by christopher on 21.02.2020.
//

#ifndef ROIWIZ_SRC_VIEW_UTIL_FILESELECTOR_H_
#define ROIWIZ_SRC_VIEW_UTIL_FILESELECTOR_H_

#include <QWidget>
#include <QComboBox>
#include <QFileDialog>

namespace e4streamer::view::util {
class FileSelector : public QWidget {
 Q_OBJECT

 public:
  Q_PROPERTY(QString path
				 READ
					 path
				 WRITE
					 setPath
				 NOTIFY
				 pathSelected)

  FileSelector(QString description,
			   QString filter,
			   bool for_saving = false,
			   QString root = QString(),
			   QWidget *parent = nullptr);
  [[nodiscard]] QString path() const;
  void setPath(const QString &path);
  void setRoot(const QString &root);
  explicit operator bool() const;
  QFileDialog::Options *operator->();

 signals:
  void pathSelected(QString path);

 private:
  void _selectPath();
  void _onSelectionChange(int index);

  QComboBox *path_;
  QString root_dir_;
  const QString description_, filter_;
  const bool for_saving_;
  QFileDialog::Options options_;
};
}

#endif //ROIWIZ_SRC_VIEW_UTIL_FILESELECTOR_H_
