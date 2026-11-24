#ifndef LR1_EXPLORER_AUDIOVIEWER_HPP
#define LR1_EXPLORER_AUDIOVIEWER_HPP

#include <QAudioSink>
#include <QBuffer>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QIcon>
#include <QTimer>

#include <LR1-Explorer/ui/viewer.hpp>

#include <LR1/formats/audio.hpp>

namespace LR1_Explorer {
    class AudioViewer : public QWidget, public virtual LR1Viewer {
        Q_OBJECT

        public:
            Q_DISABLE_COPY_MOVE(AudioViewer)

            explicit AudioViewer(QWidget* parent = nullptr);
            ~AudioViewer() override = default;

            bool load(const LR1::FileWrapper& file) override;
            bool save(const LR1::FileWrapper& file, const std::filesystem::path& outFile) override;
            void unload() override;
            QWidget* getWidget() override {return this;}

        private:
            static const QString emptyDuration;
            static const QIcon iconPlay;
            static const QIcon iconPause;

            LR1::AudioDecoder decoder;
            std::unique_ptr<QBuffer> buffer;
            std::unique_ptr<QAudioSink> player;

            QLabel* lblProgressed;
            QLabel* lblTotal;
            QProgressBar* progressBar;
            QPushButton* btnPlay;
            QTimer* timer;

            void onPlayPause() const;
            void onStateChanged(QAudio::State state) const;
            void onTimer() const;

            [[nodiscard]] QString formatFrameDuration(const qint32& frameNumber) const;
            static QString formatMicrosDuration(const qint64& microseconds);
    };
}

#endif //LR1_EXPLORER_AUDIOVIEWER_HPP
