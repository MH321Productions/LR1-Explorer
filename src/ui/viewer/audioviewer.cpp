#include <QAudioFormat>
#include <QMediaDevices>
#include <QBoxLayout>

#include <LR1-Explorer/ui/viewer/audioviewer.hpp>

using namespace std;
using namespace std::filesystem;

namespace LR1_Explorer {
    const QString AudioViewer::emptyDuration = QStringLiteral("00:00");
    const QIcon AudioViewer::iconPlay = QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart);
    const QIcon AudioViewer::iconPause = QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause);

    AudioViewer::AudioViewer(QWidget* parent) : LR1Viewer(LR1::ResourceType::Audio, QStringLiteral("WAV files (*.wav)")), QWidget(parent), buffer(make_unique<QBuffer>()), player(nullptr) {
        buffer->open(QIODeviceBase::ReadOnly);

        lblProgressed = new QLabel(emptyDuration);
        lblTotal = new QLabel(emptyDuration);
        progressBar = new QProgressBar;
        progressBar->setTextVisible(false);

        btnPlay = new QPushButton(iconPlay, QString());
        connect(btnPlay, &QPushButton::clicked, this, &AudioViewer::onPlayPause);

        auto* progressLayout = new QHBoxLayout;
        progressLayout->addWidget(lblProgressed);
        progressLayout->addWidget(progressBar);
        progressLayout->addWidget(lblTotal);

        auto* mainLayout = new QVBoxLayout;
        mainLayout->addLayout(progressLayout);
        mainLayout->addWidget(btnPlay);

        setLayout(mainLayout);

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &AudioViewer::onTimer);
    }

    bool AudioViewer::load(const LR1::FileWrapper& file) {
        if (file.type != LR1::ResourceType::Audio) return false;

        optional<LR1::PcmData> pcm = decoder.decodeWrapper(file);
        if (!pcm.has_value()) return false;

        QAudioFormat format;
        format.setChannelCount(pcm->channels);
        format.setSampleRate(pcm->sampleRate);
        format.setSampleFormat(QAudioFormat::Int16);

        lblTotal->setText(formatFrameDuration(static_cast<qint32>(pcm->data.size())));
        progressBar->setMaximum(static_cast<qint32>(format.durationForFrames(static_cast<qint32>(pcm->data.size()))));
        progressBar->setValue(0);

        if (!QMediaDevices::defaultAudioOutput().isFormatSupported(format)) {
            qWarning() << "The default audio output device \"" << QMediaDevices::defaultAudioOutput().description() << "\" doesn't support the required audio format" << format;
            return false;
        }

        buffer->buffer().append(reinterpret_cast<char*>(pcm->data.data()), static_cast<int64_t>(pcm->data.size() * sizeof(uint16_t)));
        buffer->seek(0);

        player = make_unique<QAudioSink>(format, this);
        connect(player.get(), &QAudioSink::stateChanged, this, &AudioViewer::onStateChanged);
        player->start(buffer.get());

        timer->start();

        return true;
    }

    bool AudioViewer::save(const LR1::FileWrapper& file, const path& outFile) {
        return decoder.convertWrapper(file, outFile);
    }

    void AudioViewer::unload() {
        player->stop();
        disconnect(player.get(), &QAudioSink::stateChanged, this, &AudioViewer::onStateChanged);
        buffer->buffer().clear();
    }

    void AudioViewer::onPlayPause() const {
        switch (player->state()) {
            case QAudio::ActiveState:
                player->suspend();
                break;
            case QAudio::SuspendedState:
                player->resume();
                break;
            case QAudio::IdleState:
                buffer->seek(0);
                player->resume();
                break;
            case QAudio::StoppedState:
                break;
        }
    }

    void AudioViewer::onStateChanged(const QAudio::State state) const {
        switch (state) {
            case QAudio::ActiveState:
                btnPlay->setIcon(iconPause);
                timer->start();
                break;
            case QAudio::IdleState:
            case QAudio::SuspendedState:
            case QAudio::StoppedState:
                btnPlay->setIcon(iconPlay);
                timer->stop();
                break;
        }
    }

    void AudioViewer::onTimer() const {
        const qint64 currentFrame = buffer->pos() / 2;
        const qint64 currentMicros = player->format().durationForFrames(static_cast<qint32>(currentFrame));
        progressBar->setValue(static_cast<qint32>(currentMicros));
        lblProgressed->setText(formatMicrosDuration(static_cast<qint32>(currentMicros)));
    }

    QString AudioViewer::formatFrameDuration(const qint32 &frameNumber) const {
        return formatMicrosDuration(player ? player->format().durationForFrames(frameNumber) : 0);
    }

    QString AudioViewer::formatMicrosDuration(const qint64& microseconds) {
        const int seconds = static_cast<int>(microseconds / 1'000'000);
        const int minutes = seconds / 60;
        const int secInMinutes = seconds % 60;

        return QString("%1:%2")
            .arg(minutes, 2, 10, '0')
            .arg(secInMinutes, 2, 10, '0');
    }
}
