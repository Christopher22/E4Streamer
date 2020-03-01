//
// Created by Christopher Gundler on 29.02.2020.
//

#ifndef E4STREAMER_SRC_VIEW_RECORDINGPAGE_H_
#define E4STREAMER_SRC_VIEW_RECORDINGPAGE_H_

#include <QWizardPage>
#include <QVector>

namespace e4streamer::view {
class RecordingPage : public QWizardPage {
 public:
  explicit RecordingPage(QWidget *parent = nullptr);
  void initializePage() override;
};
}

#endif //E4STREAMER_SRC_VIEW_RECORDINGPAGE_H_
